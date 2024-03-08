/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 14:04:23 by rgramati          #+#    #+#             */
/*   Updated: 2024/03/08 18:08:20 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_code;

__always_inline
void	ft_exec_mux(t_node *tree, t_fd node_fd, t_executer *ex, t_mode mode)
{
	if (!ex || !tree)
		return ;
	if (tree->command)
		ft_cmd_handler(tree, node_fd, ex, mode);
	else if (tree->token)
	{
		if (tree->token->type & TK_BINOPS)
		{
			if (!ft_strncmp(tree->token->str, "&&", 3))
				ft_and_divider(tree, node_fd, ex, mode);
			else
				ft_or_divider(tree, node_fd, ex, mode);
		}
		else
			ft_exec_pipe(tree, node_fd, ex, mode);
	}
	if (errno == EMFILE)
	{
		g_exit_code = 1;
		ft_error_message(ERR_INVFDS, "pipe");
		return ;
	}
}

void	ft_wait_pipeline(t_pid *tmp, t_executer *ex, t_mode mode)
{
	t_pid	*towait;
	int		err_code;
	int		first;

	first = 0;
	while (!(mode & 1) && ex->pids && ex->pids != tmp)
	{
		towait = ft_pid_pop(&(ex->pids));
		waitpid(towait->pid, &err_code, 0);
		ft_command_exit(err_code);
		if (!first++)
			g_exit_code = WEXITSTATUS(err_code);
		free(towait);
	}
}

void	ft_exec_pipe(t_node *tree, t_fd node_fd, t_executer *ex, t_mode mode)
{
	t_fd	fds;
	t_pid	*tmp;
	t_pipes	*tmp_pipe;

	tmp = NULL;
	if (!(mode & 1))
		tmp = ex->pids;
	ft_exec_mux(tree->left, node_fd, ex, EX_LPIPE);
	if (mode & 1)
	{
		tmp_pipe = ex->pipes;
		ft_pipes_push(&(ex->pipes), ft_init_pipes());
		fds = (t_fd){tmp_pipe->fd[0], ex->pipes->fd[1]};
		ft_exec_mux(tree->right, fds, ex, EX_RPIPE);
		ft_del_pipe(tmp_pipe);
		ex->pipes->next = NULL;
	}
	else
	{
		fds = (t_fd){ex->pipes->fd[0], node_fd.out};
		ft_exec_mux(tree->right, fds, ex, EX_RPIPE);
		ft_del_pipe(ft_pipes_pop(&(ex->pipes)));
	}
	ft_wait_pipeline(tmp, ex, mode);
}

void	ft_and_divider(t_node *tree, t_fd node_fd, t_executer *ex, t_mode mode)
{
	pid_t	child;
	int		err_code;

	err_code = 0;
	if (mode == EX_LPIPE)
	{
		ft_pipes_push(&(ex->pipes), ft_init_pipes());
		node_fd.out = ex->pipes->fd[1];
	}
	if (mode & 1)
	{
		child = fork();
		if (child == -1)
			return ;
		if (child == 0)
		{
			err_code = ft_exec_and(tree, node_fd, ex);
			ft_fork_exit(ex);
			exit(err_code);
		}
		ft_pid_push(&(ex->pids), ft_init_pid(child));
	}
	else
		ft_exec_and(tree, node_fd, ex);
}

void	ft_or_divider(t_node *tree, t_fd node_fd, t_executer *ex, t_mode mode)
{
	pid_t	child;
	int		err_code;

	err_code = 0;
	if (mode == EX_LPIPE)
	{
		ft_pipes_push(&(ex->pipes), ft_init_pipes());
		node_fd.out = ex->pipes->fd[1];
	}
	if (mode & 1)
	{
		child = fork();
		if (child == -1)
			return ;
		if (child == 0)
		{
			err_code = ft_exec_or(tree, node_fd, ex);
			ft_fork_exit(ex);
			exit(err_code);
		}
		ft_pid_push(&(ex->pids), ft_init_pid(child));
	}
	else
		ft_exec_or(tree, node_fd, ex);
}
