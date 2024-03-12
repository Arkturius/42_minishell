/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_manage_heredocs.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 13:32:59 by rgramati          #+#    #+#             */
/*   Updated: 2024/03/11 18:41:26 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_code;

void	ms_heredoc_limit(t_token *tokens, t_envvar **envp)
{
	ms_error_message(ERR_HLIMIT, NULL);
	ms_clear_token_list(tokens);
	ms_clear_env(*envp);
	exit(ERR_ERRORS);
}

void	ms_parse_line(char **line, int hd_fd, int exp)
{
	int			test;
	static int	firstline = 0;

	if (exp)
		ms_replace_vars(ms_update_env(0, NULL), line, QU_IGNORE, 1);
	test = write(hd_fd, *line, ft_strlen(*line));
	if (firstline++)
		write(hd_fd, "\n", 1);
	free(*line);
	*line = readline(P_HEREDOC);
}

int	ms_heredoc_line(char *delim, char *hd_file, int hd_fd)
{
	char	*line;
	int		exp;

	if (hd_fd == -1)
	{
		free(delim);
		return (ERR_FAILED);
	}
	line = NULL;
	exp = !(ft_strchr(delim, '"') || ft_strchr(delim, '\''));
	ms_dequote_string(&delim, QU_ZERO);
	ms_hd_holder(hd_file, 0);
	ms_hd_holder(delim, 1);
	ms_hd_holder((char *)&hd_fd, 2);
	ms_parse_line(&line, hd_fd, exp);
	while (line && ft_strncmp(line, delim, ft_strlen(delim) + 1) \
		&& !access(hd_file, F_OK))
		ms_parse_line(&line, hd_fd, exp);
	free(delim);
	free(hd_file);
	if (!line)
		return (ERR_FAILED);
	free(line);
	return (ERR_NOERRS);
}

int	ms_heredoc_exit(char *delim, char *hd_file, int err_code)
{
	int	hd_fd;

	err_code = WEXITSTATUS(err_code);
	if (err_code == 1)
	{
		ms_dequote_string(&delim, QU_ZERO);
		ms_error_message(ERR_HDSTOP, delim);
	}
	else if (err_code == 130)
	{
		unlink(hd_file);
		free(delim);
		free(hd_file);
		g_exit_code = 130;
		return (OP_HDOCKO);
	}
	hd_fd = open(hd_file, OPEN_READ);
	unlink(hd_file);
	free(delim);
	free(hd_file);
	return (hd_fd);
}

int	ms_get_heredoc(char *delim, char *hd_file)
{
	pid_t	hd_pid;
	int		hd_fd;
	int		err_code;

	err_code = 0;
	ms_signal_state(SIGHANDLER_IGN);
	hd_pid = fork();
	if (hd_pid == -1)
		return (-1);
	if (hd_pid == 0)
	{
		rl_catch_signals = 1;
		ms_signal_state(SIGHANDLER_H_D);
		hd_fd = open(hd_file, OPEN_EXCL, 0600);
		ms_clear_tree(ms_tree_holder(0, NULL));
		rl_clear_history();
		err_code = ms_heredoc_line(delim, hd_file, hd_fd);
		ms_clear_env(ms_update_env(0, NULL));
		ft_close_v(4, hd_fd, STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO);
		exit(err_code);
	}
	waitpid(hd_pid, &err_code, 0);
	ms_signal_state(SIGHANDLER_INT);
	hd_fd = ms_heredoc_exit(delim, hd_file, err_code);
	return (hd_fd);
}
