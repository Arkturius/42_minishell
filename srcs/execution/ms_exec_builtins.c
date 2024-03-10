/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exec_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 14:50:05 by rgramati          #+#    #+#             */
/*   Updated: 2024/03/10 21:20:08 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_code;

void	ms_pipe_builtin(\
int (*f)(t_command *), t_command *cmd, t_executer *ex, t_fd node_fd)
{
	pid_t	child;
	int		ret;

	child = fork();
	if (child == -1)
		return ;
	else if (child == 0)
	{
		if (f == &ms_exit)
		{
			ms_close_executer(ex);
			ex = NULL;
		}
		ms_process_redirs(cmd, node_fd);
		ret = f(cmd);
		ms_close_executer(ex);
		ms_close_tree_rec(ms_tree_holder(0, NULL));
		ex = NULL;
		if (f != &ms_exit)
			ms_fork_exit(ex);
		exit(ret);
	}
	ms_pid_push(&(ex->pids), ms_init_pid(child));
}

void	ms_wait_builtin(\
int (*f)(t_command *), t_command *cmd, t_executer *ex, t_fd node_fd)
{
	int		err_code;
	t_fd	stds;
	int		errfd;

	stds = (t_fd){STDIN_FILENO, STDOUT_FILENO};
	errfd = STDERR_FILENO;
	if (f == &ms_exit && (ft_tab_len(cmd->args) <= 2 \
						|| !ms_is_numeric(cmd->args[1])))
	{
		ms_close_tree_rec(ms_tree_holder(0, NULL));
		ms_close_executer(ex);
		ft_dprintf(STDERR_FILENO, "exit\n");
	}
	else
	{
		stds = (t_fd){dup(STDIN_FILENO), dup(STDOUT_FILENO)};
		errfd = dup(STDERR_FILENO);
		ms_process_redirs(cmd, node_fd);
	}
	err_code = f(cmd);
	if (errfd > 2)
		dup2(errfd, STDERR_FILENO);
	ms_process_redirs(NULL, stds);
	ft_close_v(3, stds.in, stds.out, errfd);
	ms_fake_pid_child(err_code, ex);
}

t_error	ms_builtin_checker(t_command *cmd, t_executer *ex)
{
	char	**tmp;

	tmp = cmd->args;
	if (!ft_strncmp(*tmp, "echo", 5))
		return (ERR_NOERRS);
	tmp++;
	if (!ft_strncmp(*(tmp - 1), "cd", 3) && !ft_strncmp(*tmp, "-", 2))
		return (ERR_NOERRS);
	while (*tmp && **tmp != '-')
		tmp++;
	if (*tmp && !ft_strncmp(*cmd->args, "exit", 5))
		return (ERR_NOERRS);
	if (*tmp)
	{
		ft_dprintf(cmd->error, ES_INVOPT, P_ERROR, **tmp, *(*tmp + 1));
		if (!ft_strncmp(*cmd->args, "env", 4))
			ms_fake_pid_child(125, ex);
		else
			ms_fake_pid_child(2, ex);
		return (ERR_INVOPT);
	}
	return (ERR_NOERRS);
}

t_error	ms_builtin(t_command *cmd, t_fd fd, t_executer *ex, t_mode mode)
{
	char		*trim;
	char		**tmp;
	static int	(*builtins[7])(t_command *) = \
	{&ms_cd, &ms_pwd, &ms_echo, &ms_env, &ms_export, &ms_unset, &ms_exit};
	static char	*builtins_str[8] = \
	{"cd", "pwd", "echo", "env", "export", "unset", "exit", NULL};

	tmp = builtins_str;
	trim = ms_backtrim(cmd->path, '/');
	while (*tmp && ft_strncmp(*tmp, trim, ft_strlen(*tmp) + 1))
		tmp++;
	free(trim);
	if (!*tmp)
		return (ERR_ERRORS);
	if (ms_builtin_checker(cmd, ex))
		return (ERR_NOERRS);
	if (mode & 1)
		ms_pipe_builtin(builtins[tmp - builtins_str], cmd, ex, fd);
	else
		ms_wait_builtin(builtins[tmp - builtins_str], cmd, ex, fd);
	ms_close_command(cmd);
	return (ERR_NOERRS);
}
