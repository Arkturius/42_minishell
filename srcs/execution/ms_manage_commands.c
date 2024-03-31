/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_manage_commands.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 12:42:43 by rgramati          #+#    #+#             */
/*   Updated: 2024/03/30 21:03:45 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			ms_replace_vars(*cmd->envp, tmp, QU_ZERO, 1);
			raw = ft_split(*(tmp++), '\026');
			ft_strtabjoin(&new_args, raw);
			continue ;
		}
		if (ft_strchr(*tmp, '*'))
			ms_replace_wildcard(tmp);
		else
			ms_dequote_string(tmp, QU_ZERO);
		ft_strapp(&new_args, ft_strdup(*(tmp++)));
	}
	ft_free_tab((void **)cmd->args);
	cmd->args = new_args;
}

void	ms_args_splitter(t_command *cmd)
{
	char	**tmp;
	char	**new;

	tmp = cmd->args;
	new = NULL;
	while (tmp && *tmp)
	{
		if (ft_strchr(*tmp, '\026'))
			ft_strtabjoin(&new, ft_split(*tmp, '\026'));
		else
			ft_strapp(&new, ft_strdup(*tmp));
		tmp++;
	}
	ft_free_tab((void **)cmd->args);
	cmd->args = new;
}

t_error	ms_command_updater(t_command *cmd)
{
	ms_args_updater(cmd);
	ms_args_splitter(cmd);
	if (!cmd->args && !cmd->redirs)
		return (ERR_NOTCMD);
	if (cmd->args && *cmd->args)
	{
		free(cmd->path);
		cmd->path = NULL;
		cmd->path = ms_get_path(*cmd->args, *(cmd->envp));
	}
	if (cmd->args && *cmd->args && **cmd->args == '.' \
		&& ft_strlen(*cmd->args) == 1)
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
