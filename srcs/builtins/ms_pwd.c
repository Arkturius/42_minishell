/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 15:35:12 by rgramati          #+#    #+#             */
/*   Updated: 2024/01/20 15:35:12 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_code;

char	*ms_trim_pwd(char *str)
{
	char	*lastb;

	lastb = ft_strrchr(str, '/');
	if (!lastb)
		return (ft_strdup(""));
	return (ft_strdup(lastb));
}

char	*ms_get_pwd(void)
{
	char	cwd[65536];

	if (getcwd(cwd, sizeof(cwd)))
		return (ft_strdup(cwd));
	return (NULL);
}

int	ms_pwd(t_command *cmd)
{
	char	cwd[65536];

	(void) cmd;
	if (getcwd(cwd, sizeof(cwd)))
		ft_printf("%s\n", cwd);
	else
	{
		ft_dprintf(STDERR_FILENO, "%spwd: getcwd syscall error\n", P_ERROR);
		return (ERR_FAILED);
	}
	return (ERR_NOERRS);
}

char	*ms_get_prompt_string(void)
{
	char	*prompt;
	char	*tmp;
	char	*pwd;

	tmp = ms_get_pwd();
	pwd = ms_backtrim(tmp, '/');
	free(tmp);
	pwd = ft_strjoin(pwd, " ~ ", 0, 0b01);
	if (!g_exit_code)
		prompt = ft_strjoin(P_SUCCESS, P_TAIL, 0, 0b00);
	else
		prompt = ft_strjoin(P_FAIL, P_TAIL, 0, 0b00);
	prompt = ft_strjoin(prompt, pwd, 0, 0b011);
	return (prompt);
}
