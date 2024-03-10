/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 14:48:51 by rgramati          #+#    #+#             */
/*   Updated: 2024/03/10 20:59:30 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_code;

void	ft_fake_pid_child(int err_code, t_executer *ex)
{
	pid_t	child;

	child = fork();
	if (child == -1)
		return ;
	if (child == 0)
	{
		ft_close_tree_rec(ft_tree_holder(0, NULL));
		ft_fork_exit(ex);
		exit(err_code);
	}
	ft_pid_push(&(ex->pids), ft_init_pid(child));
}

void	ft_process_redirs(t_command *cmd, t_fd node_fd)
{
	t_fd	fds;
	int		err;

	fds = (t_fd){node_fd.in, node_fd.out};
	err = STDERR_FILENO;
	if (cmd)
	{
		if (cmd->infile != STDIN_FILENO)
			fds.in = cmd->infile;
		if (cmd->outfile != STDOUT_FILENO)
			fds.out = cmd->outfile;
		if (cmd->error != STDERR_FILENO)
			err = cmd->error;
	}
	if (fds.in != STDIN_FILENO)
		dup2(fds.in, STDIN_FILENO);
	if (fds.out != STDOUT_FILENO)
		dup2(fds.out, STDOUT_FILENO);
	if (err != STDERR_FILENO)
		dup2(err, STDERR_FILENO);
	ft_close_command(cmd);
}

void	ft_command_exit(int err_code)
{
	if (!WIFEXITED(err_code) && WCOREDUMP(err_code))
	{
		ft_dprintf(STDERR_FILENO, "Quit (core dumped)\n");
		g_exit_code = 131;
	}
	if (WTERMSIG(err_code) == 2)
	{
		ft_dprintf(STDERR_FILENO, "\n");
		g_exit_code = 130;
	}
}

t_error	ft_check_access(char *filename, int mode)
{
	char	*name;

	name = ft_strdup(filename);
	if (!name)
		return (ERR_FAILED);
	if (access(name, F_OK))
	{
		ft_error_message(ERR_NOFORD, name);
		free(name);
		return (ERR_NOFORD);
	}
	if (access(name, mode))
	{
		ft_error_message(ERR_NOPERM, name);
		free(name);
		return (ERR_FAILED);
	}
	free(name);
	return (ERR_NOERRS);
}

void	ft_fork_exit(t_executer *ex)
{
	ft_close_executer(ex);
	rl_clear_history();
	ft_clear_env(ft_update_env(NULL));
	ft_clear_tree(ft_tree_holder(0, NULL));
}
