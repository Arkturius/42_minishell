/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_signals.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 18:51:52 by rgramati          #+#    #+#             */
/*   Updated: 2024/02/15 18:51:52 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_code;

void	ms_h_ignore(int signal)
{
	(void) signal;
}

void	ms_h_inter(int signal)
{
	if (signal == 2)
	{
		g_exit_code = 130;
		ft_printf("^C\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	ms_h_quote(int signal)
{
	int	fd;

	fd = 0;
	ft_printf("\001\033[%dC\002", (int) ft_strlen(rl_prompt) + rl_point - 15);
	if (signal == 2)
	{
		ft_printf("^C\n");
		free(ms_dq_holder(NULL, 0));
		fd = *(int *)ms_dq_holder(NULL, 1);
		ft_close_v(4, fd, STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO);
		g_exit_code = 130;
		exit(130);
	}
}

void	ms_h_heredoc(int signal)
{
	int	fd;

	fd = 0;
	ft_printf("\001\033[%dC\002", (int) ft_strlen(rl_prompt) + rl_point - 15);
	if (signal == 2)
	{
		ft_printf("^C\n");
		unlink(ms_hd_holder(NULL, 0));
		ms_clear_env(ms_update_env(0, NULL));
		free(ms_hd_holder(NULL, 0));
		free(ms_hd_holder(NULL, 1));
		fd = *(int *)ms_hd_holder(NULL, 2);
		ft_close_v(4, fd, STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO);
		g_exit_code = 130;
		exit(130);
	}
}

void	ms_signal_state(int state)
{
	static struct termios	termios_data;
	static void				(*handlers[4])(int) = {\
	ms_h_ignore, ms_h_inter, ms_h_quote, ms_h_heredoc};

	if (state)
		tcgetattr(0, &termios_data);
	if (!state)
		termios_data.c_lflag = termios_data.c_lflag | ECHOCTL;
	else
		termios_data.c_lflag = termios_data.c_lflag & (~ECHOCTL);
	tcsetattr(0, 0, &termios_data);
	signal(SIGINT, handlers[state]);
	signal(SIGQUIT, handlers[state]);
}
