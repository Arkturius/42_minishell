/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exec_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 14:04:23 by rgramati          #+#    #+#             */
/*   Updated: 2024/03/13 20:35:14 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_code;

__always_inline
void	ms_exec_mux(t_node *tree, t_fd node_fd, t_executer *ex, t_mode mode)
{
	if (!ex || !tree)
		return ;
	if (tree->command)
		ms_cmd_handler(tree, node_fd, ex, mode);
	else if (tree->token)
	{
		if (tree->token->type & TK_BINOPS)
			ms_ops_divider(tree, node_fd, ex, mode);
		else
			ms_exec_pipe(tree, node_fd, ex, mode);
	}
	if (errno == EMFILE)
	{
		g_exit_code = 1;
		ms_error_message(ERR_INVFDS, "pipe");
		return ;
	}
}

void	ms_wait_pipeline(t_pid *tmp, t_executer *ex, t_mode mode)
{
	t_pid	*towait;
	int		last_err;
	int		err_code;
	int		first;

	first = 0;
	while (!(mode & 1) && ex->pids && ex->pids != tmp)
	{
		towait = ms_pid_pop(&(ex->pids));
		waitpid(towait->pid, &err_code, 0);
		ms_command_exit(err_code);
		if (!first++)
			last_err = WEXITSTATUS(err_code);
		free(towait);
	}
	g_exit_code = last_err;
}

void	ms_exec_pipe(t_node *tree, t_fd node_fd, t_executer *ex, t_mode mode)
{
	t_fd	fds;
	t_pid	*tmp;
	t_pipes	*tmp_pipe;

	tmp = NULL;
	if (!(mode & 1))
		tmp = ex->pids;
	ms_exec_mux(tree->left, node_fd, ex, EX_LPIPE);
	if (mode & 1)
	{
		tmp_pipe = ex->pipes;
		ms_pipes_push(&(ex->pipes), ms_init_pipes());
		fds = (t_fd){tmp_pipe->fd[0], ex->pipes->fd[1]};
		ms_exec_mux(tree->right, fds, ex, EX_RPIPE);
		ms_del_pipe(tmp_pipe);
		ex->pipes->next = NULL;
	}
	else
	{
		fds = (t_fd){ex->pipes->fd[0], node_fd.out};
		ms_exec_mux(tree->right, fds, ex, EX_RPIPE);
		ms_del_pipe(ms_pipes_pop(&(ex->pipes)));
	}
	ms_wait_pipeline(tmp, ex, mode);
}
