/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_manage_commands.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 12:42:43 by rgramati          #+#    #+#             */
/*   Updated: 2024/03/10 21:23:19 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ms_quoted_split(char *str, char *sep)
{
	t_qstate	qs;
	char		**new;
	char		*tmp;
	char		*hold;

	new = NULL;
	qs = QU_ZERO;
	if (!str || !sep || !*str)
		return (ft_strtab(ft_strdup("")));
	tmp = str;
	while (*str && *tmp)
	{
		tmp = str;
		while (*tmp && (!ft_strchr(sep, *tmp) || qs != QU_ZERO))
			ms_qs_update(*(tmp++), &qs);
		hold = ft_strndup(str, tmp - str);
		ft_strapp(&new, hold);
		str = tmp;
		while (*str && ft_strchr(sep, *str))
			str++;
	}
	return (new);
}

void	ms_args_updater(t_command *cmd)
{
	char	**new_args;
	char	**raw;
	char	**tmp;

	tmp = cmd->args;
	new_args = NULL;
	while (tmp && *tmp)
	{
		if (ft_strchr(*tmp, '$'))
		{
			raw = ms_replace_vars(*cmd->envp, tmp, QU_ZERO, 1);
			ft_strapp(&new_args, ft_strsjoin(raw, NULL, 0b01));
			tmp++;
			continue ;
		}
		if (ft_strncmp(*(cmd->args), "export", 7))
		{
			if (ms_verify_wildcard(*tmp, QU_ZERO))
				ms_replace_wildcard(tmp);
		}
		ms_dequote_string(tmp, QU_ZERO);
		ft_strapp(&new_args, ft_strdup(*(tmp++)));
	}
	ft_free_tab((void **)cmd->args);
	cmd->args = new_args;
}

t_error	ms_command_updater(t_command *cmd)
{
	ms_args_updater(cmd);
	if (cmd->args && *cmd->args && \
		**cmd->args != '.' && !ft_strchr(*cmd->args, '/'))
	{
		free(cmd->path);
		cmd->path = NULL;
		cmd->path = ms_get_path(*cmd->args, *(cmd->envp));
	}
	if ((!cmd->path && !cmd->redirs) || \
		(**cmd->args == '.' && ft_strlen(*cmd->args) == 1))
		return (ERR_NOTCMD);
	return (ERR_NOERRS);
}

t_error	ms_manage_heredocs(t_node *nd, int *hd)
{
	t_error	err;

	if (!nd)
		return (ERR_ERRORS);
	err = ERR_NOERRS;
	if (*hd)
		return (ERR_HDSTOP);
	if (!nd->command)
	{
		err |= ms_manage_heredocs(nd->left, hd);
		err |= ms_manage_heredocs(nd->right, hd);
		return (err);
	}
	err = ms_open_heredocs(nd->command);
	return (err);
}
