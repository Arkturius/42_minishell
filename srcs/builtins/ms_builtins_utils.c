/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtins_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 19:49:54 by rgramati          #+#    #+#             */
/*   Updated: 2024/03/27 17:03:57 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

long	ms_abs(long n)
{
	if (n < 0)
		return (-n);
	return (n);
}

int	ms_is_numeric(char *str)
{
	int		sign;
	char	*tmp;

	sign = 0;
	tmp = str;
	while (ft_isspace(*str))
		str++;
	if (*str == '-' || *str == '+')
	{
		str++;
		sign = 1;
	}
	while (ft_isdigit(*(str)))
		str++;
	if (sign)
		return (!*str);
	return (!*str && (str - tmp));
}

char	*ms_backtrim(char *str, char c)
{
	char	*tmp;
	char	*chr;

	chr = ft_strrchr(str, c);
	tmp = NULL;
	if (chr && *chr && *(chr + 1))
		tmp = ft_strndup(chr + 1, ft_strlen(str) - (chr - str));
	return (tmp);
}
