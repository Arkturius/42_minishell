/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exec_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 20:45:23 by rgramati          #+#    #+#             */
/*   Updated: 2024/03/31 16:02:28 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_code;

void	ms_exec_cmd(t_command *cmd, t_fd node_fd, t_executer *ex)
{
	char	**env;
	pid_t	child;

	child = fork();
	if (child == -1)
		return ;
	if (child == 0)
	{
		env = ms_get_var_strs(*(cmd->envp), 0);
		ms_process_redirs(cmd, node_fd);
		ms_close_pipes(ex->pipes);
		ms_close_tree_rec(ms_tree_holder(0, NULL));
		execve(cmd->path, cmd->args, env);
	}
	ms_close_command(cmd);
	ms_pid_push(&(ex->pids), ms_init_pid(child));
}

t_error	ms_command_startup(t_command *cmd, t_executer *ex)
{
	int	hd_last;

	hd_last = 0;
	if (ms_command_updater(cmd))
	{
		ms_fake_pid_child(0, ex);
		return (ERR_NOTCMD);
	}
	if (ms_open_outputs(cmd) || ms_open_inputs(cmd, &hd_last))
	{
		ms_fake_pid_child(1, ex);
		return (ERR_FAILED);
	}
	ms_connect_input(cmd, hd_last);
	if (!cmd->path && !*cmd->args && cmd->redirs)
		ms_fake_pid_child(0, ex);
	if (!cmd->path && !*cmd->args && cmd->redirs)
		return (ERR_FAILED);
	return (ERR_NOERRS);
}

t_error	ms_cmd_error(t_command *cmd, t_executer *ex, int status, struct stat st)
{
	if (S_ISDIR(st.st_mode))
		ms_error_message(ERR_ISADIR, cmd->path);
	else if (status == -1 && errno == ENOTDIR)
		ms_error_message(ERR_ISNDIR, cmd->path);
	else if (status != -1 && access(cmd->path, X_OK))
		ms_error_message(ERR_NOPERM, cmd->path);
	else if (status == -1 && errno == ENOENT)
	{
		ms_error_message(ERR_NOFORD, cmd->path);
		ms_fake_pid_child(127, ex);
		return (ERR_FAILED);
	}
	else if (S_ISFIFO(st.st_mode))
		ms_error_message(ERR_NOPERM, cmd->path);
	else
		return (ERR_NOERRS);
	ms_fake_pid_child(126, ex);
	return (ERR_FAILED);
}

t_error	ms_command_checker(t_command *cmd, t_executer *ex)
{
	char		*err_str;
	char		**path_str;
	int			status;
	struct stat	st;

	err_str = NULL;
	if (cmd->path)
	{
		status = stat(cmd->path, &st);
		path_str = &(cmd->path);
	}
	else
	{
		status = stat(*cmd->args, &st);
		path_str = cmd->args;
	}
	ms_dequote_string(path_str, QU_ZERO);
	if (ms_cmd_error(cmd, ex, status, st))
		return (ERR_FAILED);
	return (ERR_NOERRS);
}

void	ms_cmd_handler(t_node *tree, t_fd node_fd, t_executer *ex, t_mode mode)
{
	char		*err_str;
	t_command	*cmd;

	err_str = NULL;
	cmd = tree->command;
	if (mode == EX_LPIPE)
	{
		ms_pipes_push(&(ex->pipes), ms_init_pipes());
		node_fd.out = ex->pipes->fd[1];
	}
	if (ms_command_startup(cmd, ex))
		return ;
	if (!ms_builtin(cmd, node_fd, ex, mode))
		return ;
	if (!cmd->path)
	{
		ms_error_message(ERR_NOTCMD, *cmd->args);
		ms_fake_pid_child(127, ex);
		return ;
	}
	if (ms_command_checker(cmd, ex))
		return ;
	ms_exec_cmd(cmd, node_fd, ex);
}
