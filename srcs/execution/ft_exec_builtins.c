/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 14:50:05 by rgramati          #+#    #+#             */
/*   Updated: 2024/03/10 20:59:46 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_code;

void	ft_pipe_builtin(\
int (*f)(t_command *), t_command *cmd, t_executer *ex, t_fd node_fd)
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
			ex = NULL;
		}
		ft_process_redirs(cmd, node_fd);
		ret = f(cmd);
		ft_close_executer(ex);
		ft_close_tree_rec(ft_tree_holder(0, NULL));
		ex = NULL;
		if (f != &ft_exit)
			ft_fork_exit(ex);
		exit(ret);
	}
	ft_pid_push(&(ex->pids), ft_init_pid(child));
}

void	ft_wait_builtin(\
int (*f)(t_command *), t_command *cmd, t_executer *ex, t_fd node_fd)
{
	int		err_code;
	t_fd	stds;
	int		errfd;

	stds = (t_fd){STDIN_FILENO, STDOUT_FILENO};
	errfd = STDERR_FILENO;
	if (f == &ft_exit && (ft_tab_len(cmd->args) <= 2 \
						|| !ft_is_numeric(cmd->args[1])))
	{
		ft_close_tree_rec(ft_tree_holder(0, NULL));
		ft_close_executer(ex);
		ft_dprintf(STDERR_FILENO, "exit\n");
	}
	else
	{
		stds = (t_fd){dup(STDIN_FILENO), dup(STDOUT_FILENO)};
		errfd = dup(STDERR_FILENO);
		ft_process_redirs(cmd, node_fd);
	}
	err_code = f(cmd);
	if (errfd > 2)
		dup2(errfd, STDERR_FILENO);
	ft_process_redirs(NULL, stds);
	ft_close_v(3, stds.in, stds.out, errfd);
	ft_fake_pid_child(err_code, ex);
}

t_error	ft_builtin_checker(t_command *cmd, t_executer *ex)
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
			ft_fake_pid_child(125, ex);
		else
			ft_fake_pid_child(2, ex);
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
	if (ft_builtin_checker(cmd, ex))
		return (ERR_NOERRS);
	if (mode & 1)
		ft_pipe_builtin(builtins[tmp - builtins_str], cmd, ex, fd);
	else
		ft_wait_builtin(builtins[tmp - builtins_str], cmd, ex, fd);
	ft_close_command(cmd);
	return (ERR_NOERRS);
}
