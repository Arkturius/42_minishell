/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_manage_files.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 16:02:28 by rgramati          #+#    #+#             */
/*   Updated: 2024/03/09 18:40:38 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_error	ft_open_heredocs(t_command *cmd)
{
	t_error	err;
	t_redir	*tmp;
	char	*hd_file;

	err = ERR_NOERRS;
	tmp = cmd->redirs;
	while (tmp && cmd->heredoc != -1)
	{
		if (tmp->type == RD_HEREDOC)
		{
			hd_file = ft_get_temp_file(".heredoc", 16);
			if (cmd->heredoc > 2)
				close(cmd->heredoc);
			cmd->heredoc = ft_get_heredoc(ft_strdup(tmp->file), hd_file);
			if (cmd->heredoc == OP_HDOCKO)
				return (ERR_HDSTOP);
		}
		tmp = tmp->next;
	}
	return (cmd->heredoc == OP_FILEKO);
}

t_error	ft_file_checker(t_command *cmd, char **file, int mode)
{
	char	**files;
	char	*var;

	var = ft_strdup(*file);
	if (ft_strchr(*file, '$'))
		ft_replace_vars(*cmd->envp, file, QU_ZERO, 0);
	files = ft_quoted_split(*file, " ");
	if (ft_tab_len(files) > 1)
	{
		ft_error_message(ERR_AMBRED, var);
		free(var);
		ft_free_tab((void **) files);
		return (ERR_AMBRED);
	}
	ft_dequote_string(file, QU_ZERO);
	free(var);
	ft_free_tab((void **) files);
	if (mode == OPEN_READ)
		return (ft_check_access(*file, R_OK));
	return (ERR_NOERRS);
}

t_error	ft_open_file(t_command *cmd, char **file, int mode)
{
	int	*fd;

	if (ft_file_checker(cmd, file, mode))
		return (ERR_AMBRED);
	if (mode == OPEN_READ)
		fd = &(cmd->infile);
	if ((mode == OPEN_CREATE || mode == OPEN_APPEND))
		fd = &(cmd->outfile);
	if (*fd > 2)
		close(*fd);
	if (mode != OPEN_READ && *fd != OP_FILEKO)
		*fd = open(*file, mode, 0644);
	else if (*fd != -1)
		*fd = open(*file, mode);
	if (*fd == -1)
	{
		if (errno == ENFILE)
			ft_error_message(ERR_INVFDS, *file);
		else
			ft_error_message(ERR_NOFORD, *file);
		return (ERR_INVFDS);
	}
	return (ERR_NOERRS);
}

t_error	ft_open_outputs(t_command *cmd)
{
	t_redir	*tmp;
	t_error	op;

	tmp = cmd->redirs;
	op = ERR_NOERRS;
	while (tmp && cmd->outfile != OP_FILEKO && op == ERR_NOERRS)
	{
		if (tmp->type == RD_INFILES && access(tmp->file, F_OK))
			break ;
		if (tmp->type == RD_OUTPUTS)
			op = ft_open_file(cmd, &(tmp->file), OPEN_CREATE);
		else if (tmp->type == RD_APPENDS)
			op = ft_open_file(cmd, &(tmp->file), OPEN_APPEND);
		if (cmd->outfile != OP_FILEKO)
			tmp = tmp->next;
	}
	if (op)
		return (op);
	return (cmd->outfile == OP_FILEKO && access(tmp->file, R_OK));
}

t_error	ft_open_inputs(t_command *cmd, int *hd_last)
{
	t_redir	*tmp;
	t_error	op;

	if (!cmd->redirs)
		return (ERR_NOERRS);
	tmp = cmd->redirs;
	op = ERR_NOERRS;
	*hd_last = 0;
	while (tmp && cmd->infile != OP_FILEKO && op == ERR_NOERRS)
	{
		if (tmp->type == RD_INFILES)
		{
			*hd_last = 0;
			op = ft_open_file(cmd, &(tmp->file), OPEN_READ);
		}
		if (tmp->type != RD_OUTPUTS && tmp->type != RD_APPENDS)
			*hd_last |= (tmp->type == RD_HEREDOC);
		tmp = tmp->next;
	}
	if (op)
		return (op);
	return (cmd->infile == OP_FILEKO);
}
