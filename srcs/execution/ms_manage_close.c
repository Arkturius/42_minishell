/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_manage_close.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 17:43:40 by rgramati          #+#    #+#             */
/*   Updated: 2024/03/31 15:58:57 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_error	ms_open_file(int *fd, char **file, int mode)
{
	if (ms_file_checker(file, mode))
		return (ERR_AMBRED);
	if (*fd > 2)
		close(*fd);
	if (mode != OPEN_READ && *fd != OP_FILEKO)
		*fd = open(*file, mode, 0644);
	else if (*fd != -1)
		*fd = open(*file, mode);
	if (*fd == -1)
	{
		if (errno == ENFILE)
			ms_error_message(ERR_INVFDS, *file);
		else
			ms_error_message(ERR_NOFORD, *file);
		return (ERR_INVFDS);
	}
	return (ERR_NOERRS);
}

void	ms_close_command(t_command *command)
{
	if (!command)
		return ;
	if (command->infile > 2)
		close(command->infile);
	if (command->outfile > 2)
		close(command->outfile);
	if (command->error > 2)
		close(command->error);
	if (command->heredoc > 2)
		close(command->heredoc);
}

void	ms_close_tree_rec(t_node *tree)
{
	if (!tree)
		return ;
	if (tree->left)
		ms_close_tree_rec(tree->left);
	if (tree->right)
		ms_close_tree_rec(tree->right);
	if (tree->command)
		ms_close_command(tree->command);
}

void	ms_close_executer(t_executer *ex)
{
	t_pipes	*tmp_pipe;
	t_pid	*tmp_pid;

	if (!ex)
		return ;
	while (ex->pipes)
	{
		tmp_pipe = ms_pipes_pop(&(ex->pipes));
		if (tmp_pipe->fd[0] > 2)
			close(tmp_pipe->fd[0]);
		if (tmp_pipe->fd[1] > 2)
			close(tmp_pipe->fd[1]);
		free(tmp_pipe);
	}
	while (ex->pids)
	{
		tmp_pid = ms_pid_pop(&(ex->pids));
		free(tmp_pid);
	}
	free(ex);
}

void	ms_close_pipes(t_pipes *tmp_pipe)
{
	if (!tmp_pipe)
		return ;
	while (tmp_pipe)
	{
		if (tmp_pipe->fd[0] > 2)
			close(tmp_pipe->fd[0]);
		if (tmp_pipe->fd[1] > 2)
			close(tmp_pipe->fd[1]);
		tmp_pipe = tmp_pipe->next;
	}
}
