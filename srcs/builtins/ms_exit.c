/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 07:38:34 by rgramati          #+#    #+#             */
/*   Updated: 2024/03/29 17:40:01 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_code;

long	ms_exit_atoi(char *str, t_command *cmd)
{
	long	n;
	int		sign;

	n = 0;
	sign = 0;
	while (ft_isspace(*str))
		str++;
	if (!ft_strncmp(str, "-9223372036854775808", 21))
		return (0);
	if (*str == '-' || *str == '+')
		sign = (*(str++) == '-');
	while (ft_isdigit(*str) && (n * 10 + (int){*(str) - '0'} >= n))
		n = n * 10 + (int){*(str++) - '0'};
	if (ft_isdigit(*str))
		ms_exit_manager(ERR_ERRORS, ERR_NOTNUM, cmd);
	if (sign)
		return ((256 - n) % 256);
	return (n % 256);
}

void	ms_exit_manager(int exit_code, int ec, t_command *cmd)
{
	char		*msg;

	if (ec == ERR_NOTNUM)
	{
		msg = ft_strjoin("exit: ", cmd->args[1], NULL, 0b00);
		ms_error_message(ERR_NOTNUM, msg);
		free(msg);
	}
	if (ec == ERR_TMARGS)
	{
		ms_error_message(ERR_TMARGS, "exit");
		return ;
	}
	rl_clear_history();
	if (cmd)
		ms_clear_env(*(cmd->envp));
	ms_clear_tree(ms_tree_holder(0, NULL));
	exit(exit_code);
}

int	ms_exit(t_command *cmd)
{
	int			argc;

	argc = 0;
	ms_close_command(cmd);
	if (cmd)
		argc = ft_tab_len(cmd->args) - 1;
	else
		ft_dprintf(STDERR_FILENO, "exit\n");
	if (!cmd || !argc)
		ms_exit_manager(g_exit_code, ERR_NOERRS, cmd);
	ms_exit_atoi(cmd->args[1], cmd);
	if (!ms_is_numeric(cmd->args[1]))
		ms_exit_manager(ERR_ERRORS, ERR_NOTNUM, cmd);
	if (argc > 1)
		ms_exit_manager(ERR_FAILED, ERR_TMARGS, cmd);
	else
		ms_exit_manager(ms_exit_atoi(cmd->args[1], cmd), ERR_NOERRS, cmd);
	return (argc > 1);
}
