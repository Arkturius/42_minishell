/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exec_binops.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 13:48:29 by rgramati          #+#    #+#             */
/*   Updated: 2024/03/10 21:19:12 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_code;

int	ms_wait_and_or(t_executer *ex)
{
	t_pid	*towait;
	int		err_code;

	towait = ms_pid_pop(&(ex->pids));
	err_code = 0;
	if (towait)
	{
		ms_close_pipes(ex->pipes);
		waitpid(towait->pid, &err_code, 0);
		ms_command_exit(err_code);
		err_code = WEXITSTATUS(err_code);
	}
	else
		err_code = g_exit_code;
	free(towait);
	return (err_code);
}

int	ms_exec_ops(t_node *tree, t_fd node_fd, t_executer *ex)
{
	if (node_fd.in != STDIN_FILENO)
		dup2(node_fd.in, STDIN_FILENO);
	if (node_fd.out != STDOUT_FILENO)
		dup2(node_fd.out, STDOUT_FILENO);
	ms_exec_mux(tree->left, node_fd, ex, EX_LWAIT);
	g_exit_code = ms_wait_and_or(ex);
	if (!ft_strncmp(tree->token->str, "&&", 3) && g_exit_code == 0)
	{
		ms_exec_mux(tree->right, node_fd, ex, EX_RWAIT);
		g_exit_code = ms_wait_and_or(ex);
	}
	else if (!ft_strncmp(tree->token->str, "||", 3) && g_exit_code != 0)
	{
		ms_exec_mux(tree->right, node_fd, ex, EX_RWAIT);
		g_exit_code = ms_wait_and_or(ex);
	}
	else if (!ft_strncmp(tree->token->str, ";", 2))
	{
		ms_exec_mux(tree->right, node_fd, ex, EX_RWAIT);
		g_exit_code = ms_wait_and_or(ex);
	}
	else
		ms_fake_pid_child(g_exit_code, ex);
	return (g_exit_code);
}

void	ms_ops_divider(t_node *tree, t_fd node_fd, t_executer *ex, t_mode mode)
{
	pid_t	child;
	int		err_code;

	err_code = 0;
	if (mode == EX_LPIPE)
	{
		ms_pipes_push(&(ex->pipes), ms_init_pipes());
		node_fd.out = ex->pipes->fd[1];
	}
	if (mode & 1)
	{
		child = fork();
		if (child == -1)
			return ;
		if (child == 0)
		{
			err_code = ms_exec_ops(tree, node_fd, ex);
			ms_fork_exit(ex);
			exit(err_code);
		}
		ms_pid_push(&(ex->pids), ms_init_pid(child));
	}
	else
		ms_exec_ops(tree, node_fd, ex);
}
