/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 14:50:05 by rgramati          #+#    #+#             */
/*   Updated: 2024/03/06 19:19:29 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_code;

void	ft_pipe_builtin(int (*f)(t_command *), t_command *cmd, t_executer *ex)
{
	pid_t	child;
	int		ret;

	child = fork();
	if (child == -1)
		return ;
	else if (child == 0)
	{
		if (f == &ft_exit)
		{
			ft_close_executer(ex);
			free(ex);
			ex = NULL;
		}
		ret = f(cmd);
		ft_close_executer(ex);
		ft_close_tree_rec(ft_tree_holder(0, NULL));
		free(ex);
		ex = NULL;
		if (f != &ft_exit)
			ft_fork_exit(ex);
		exit(ret);
	}
	ft_close_command(cmd);
	ft_pid_push(&(ex->pids), ft_init_pid(child));
}

void	ft_wait_builtin(int (*f)(t_command *), t_command *cmd, t_executer *ex)
{
	int	err_code;

	if (f == &ft_exit && ft_tab_len(cmd->args) <= 2)
	{
		ft_close_tree_rec(ft_tree_holder(0, NULL));
		ft_close_executer(ex);
		free(ex);
		ft_dprintf(2, "exit\n");
	}
	err_code = f(cmd);
	ft_fake_pid_child(err_code, ex);
	ft_close_command(cmd);
}

t_error	ft_builtin_checker(t_command *cmd, t_fd fd)
{
	char	**tmp;

	tmp = cmd->args;
	if (cmd->infile == STDIN_FILENO && fd.in != STDIN_FILENO)
		cmd->infile = fd.in;
	if (cmd->outfile == STDOUT_FILENO && fd.out != STDOUT_FILENO)
		cmd->outfile = fd.out;
	if (!ft_strncmp(*tmp, "echo", 5))
		return (ERR_NOERRS);
	tmp++;
	if (!ft_strncmp(*(tmp - 1), "cd", 3) && !ft_strncmp(*tmp, "-", 2))
		return (ERR_NOERRS);
	while (*tmp && **tmp != '-')
		tmp++;
	if (*tmp)
	{
		ft_error_message(ERR_INVOPT, *tmp);
		g_exit_code = 125;
		return (ERR_INVOPT);
	}
	return (ERR_NOERRS);
}

t_error	ft_builtin(t_command *cmd, t_fd fd, t_executer *ex, t_mode mode)
{
	char		*trim;
	char		**tmp;
	static int	(*builtins[7])(t_command *) = \
	{&ft_cd, &ft_pwd, &ft_echo, &ft_env, &ft_export, &ft_unset, &ft_exit};
	static char	*builtins_str[8] = \
	{"cd", "pwd", "echo", "env", "export", "unset", "exit", NULL};

	tmp = builtins_str;
	trim = ft_backtrim(cmd->path, '/');
	while (*tmp && ft_strncmp(*tmp, trim, ft_strlen(*tmp) + 1))
		tmp++;
	free(trim);
	if (!*tmp)
		return (ERR_ERRORS);
	if (mode == EX_LPIPE)
		ft_pipes_push(&(ex->pipes), ft_init_pipes());
	if (mode == EX_LPIPE)
		fd.out = ex->pipes->fd[1];
	if (ft_builtin_checker(cmd, fd))
		return (ERR_NOERRS);
	if (mode & 1)
		ft_pipe_builtin(builtins[tmp - builtins_str], cmd, ex);
	else
		ft_wait_builtin(builtins[tmp - builtins_str], cmd, ex);
	return (ERR_NOERRS);
}
