/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_error_handling.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 22:00:02 by rgramati          #+#    #+#             */
/*   Updated: 2024/03/30 16:07:00 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_errors_end(t_error err, char *str)
{
	if (err == ERR_NOTCMD)
	{
		ft_dprintf(STDERR_FILENO, "%s", P_ERROR);
		if (str)
			ft_dprintf(STDERR_FILENO, "%s", str);
		ft_dprintf(STDERR_FILENO, ": command not found\n");
	}
	if (err == ERR_SYNTXQ)
		ft_dprintf(STDERR_FILENO, ES_SYNTXQ, P_ERROR, str);
	if (err == ERR_SYNTXT)
	{
		if (str)
			ft_dprintf(STDERR_FILENO, ES_SYNTXT, P_ERROR, str);
		else
			ft_dprintf(STDERR_FILENO, ES_SYNTXL, P_ERROR);
	}
	if (err == ERR_HLIMIT)
		ft_dprintf(STDERR_FILENO, ES_HLIMIT, P_ERROR);
	if (err == ERR_INVFDS)
		ft_dprintf(STDERR_FILENO, ES_INVFDS, P_ERROR, str);
	if (err == ERR_AMBRED)
		ft_dprintf(STDERR_FILENO, ES_AMBRED, P_ERROR, str);
	ft_dprintf(STDERR_FILENO, "\001\033[0m\002");
}

void	ms_error_message(t_error err, char *str)
{
	if (err == ERR_NOTSET)
		ft_dprintf(STDERR_FILENO, ES_NOTSET, P_ERROR, str);
	if (err == ERR_NOFORD)
		ft_dprintf(STDERR_FILENO, ES_NOFORD, P_ERROR, str);
	if (err == ERR_TMARGS)
		ft_dprintf(STDERR_FILENO, ES_TMARGS, P_ERROR, str);
	if (err == ERR_NOTNUM)
		ft_dprintf(STDERR_FILENO, ES_NOTNUM, P_ERROR, str);
	if (err == ERR_NOTVAL)
		ft_dprintf(STDERR_FILENO, ES_NOTVAL, P_ERROR, str);
	if (err == ERR_NOPERM)
		ft_dprintf(STDERR_FILENO, ES_NOPERM, P_ERROR, str);
	if (err == ERR_ISADIR)
		ft_dprintf(STDERR_FILENO, ES_ISADIR, P_ERROR, str);
	if (err == ERR_ISNDIR)
		ft_dprintf(STDERR_FILENO, ES_ISNDIR, P_ERROR, str);
	if (err == ERR_INVOPT)
		ft_dprintf(STDERR_FILENO, ES_INVOPT, P_ERROR, *str, *(str + 1));
	if (err == ERR_HDSTOP)
		ft_dprintf(STDERR_FILENO, ES_HDSTOP, P_WARNING, str);
	if (err == ERR_DQSTOP)
		ft_dprintf(STDERR_FILENO, ES_DQSTOP, P_WARNING, str);
	if (err == ERR_NOARGS)
		ft_dprintf(STDERR_FILENO, ES_NOARGS, P_ERROR, str);
	ms_errors_end(err, str);
}
