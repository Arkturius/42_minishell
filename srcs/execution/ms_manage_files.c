/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_manage_files.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 16:02:28 by rgramati          #+#    #+#             */
/*   Updated: 2024/03/31 15:55:53 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_error	ms_open_heredocs(t_command *cmd)
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
			hd_file = ms_get_temp_file(".heredoc", 16);
			if (cmd->heredoc > 2)
				close(cmd->heredoc);
			cmd->heredoc = ms_get_heredoc(ft_strdup(tmp->file), hd_file);
			if (cmd->heredoc == OP_HDOCKO)
				return (ERR_HDSTOP);
		}
		tmp = tmp->next;
	}
	return (cmd->heredoc == OP_FILEKO);
}

t_error	ms_file_checker(char **file, int mode)
{
	char	**files;
	char	*var;

	var = ft_strdup(*file);
	if (ft_strchr(*file, '$'))
		ms_replace_vars(ms_update_env(0, NULL), file, QU_ZERO, 0);
	ms_replace_wildcard(file);
	files = ft_split(*file, '\026');
	if (ft_tab_len(files) > 1)
	{
		ms_error_message(ERR_AMBRED, var);
		free(var);
		ft_free_tab((void **) files);
		return (ERR_AMBRED);
	}
	ms_dequote_string(file, QU_ZERO);
	free(var);
	ft_free_tab((void **) files);
	if (mode == OPEN_READ)
		return (ms_check_access(*file, R_OK));
	return (ERR_NOERRS);
}

t_error	ms_input_checker(char *file)
{
	char	**files;
	char	*var;

	var = ft_strdup(file);
	if (ft_strchr(file, '$'))
		ms_replace_vars(ms_update_env(0, NULL), &file, QU_ZERO, 0);
	ms_replace_wildcard(&file);
	files = ft_split(file, '\026');
	if (ft_tab_len(files) > 1)
	{
		free(var);
		free(file);
		ft_free_tab((void **) files);
		return (ERR_AMBRED);
	}
	ms_dequote_string(&file, QU_ZERO);
	free(var);
	ft_free_tab((void **) files);
	if (!access(file, R_OK))
	{
		free(file);
		return (ERR_NOERRS);
	}
	free(file);
	return (ERR_NOFORD);
}

t_error	ms_open_outputs(t_command *cmd)
{
	t_redir	*tmp;
	t_error	op;

	tmp = cmd->redirs;
	op = ERR_NOERRS;
	while (tmp && cmd->outfile != OP_FILEKO && op == ERR_NOERRS)
	{
		if (tmp->type == RD_INFILES && ms_input_checker(ft_strdup(tmp->file)))
			break ;
		if (tmp->type == RD_OUTPUTS)
			op = ms_open_file(&(cmd->outfile), &(tmp->file), OPEN_CREATE);
		else if (tmp->type == RD_APPENDS)
			op = ms_open_file(&(cmd->outfile), &(tmp->file), OPEN_APPEND);
		else if (tmp->type == RD_ERRSOUT)
			op = ms_open_file(&(cmd->error), &(tmp->file), OPEN_CREATE);
		if (cmd->outfile != OP_FILEKO)
			tmp = tmp->next;
	}
	if (op)
		return (op);
	return (cmd->outfile == OP_FILEKO && access(tmp->file, R_OK));
}

t_error	ms_open_inputs(t_command *cmd, int *hd_last)
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
			op = ms_open_file(&(cmd->infile), &(tmp->file), OPEN_READ);
		}
		if (tmp->type != RD_OUTPUTS && tmp->type != RD_APPENDS)
			*hd_last |= (tmp->type == RD_HEREDOC);
		tmp = tmp->next;
	}
	if (op)
		return (op);
	return (cmd->infile == OP_FILEKO);
}
