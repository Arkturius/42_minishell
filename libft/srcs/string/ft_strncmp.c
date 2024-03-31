/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 02:17:18 by rgramati          #+#    #+#             */
/*   Updated: 2024/03/31 14:27:19 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(char *s1, char *s2, size_t n)
{
	if (!s1 || !s2)
		return (s1 != s2);
	while (*s1 && *s1 == *s2 && n--)
	{
		s1++;
		s2++;
	}
	if (!n)
		return (0);
	return (*s1 - *s2);
}

int	ft_strrncmp(char *s1, char *s2, size_t n)
{
	char	*tmp_1;
	char	*tmp_2;

	if (!s1 || !s2)
		return (1);
	tmp_1 = s1 + ft_strlen(s1) - !!ft_strlen(s1);
	tmp_2 = s2 + ft_strlen(s2) - !!ft_strlen(s2);
	while (tmp_1 >= s1 && tmp_2 >= s2 && *tmp_1 == *tmp_2 && n--)
	{
		tmp_1--;
		tmp_2--;
	}
	if (!n)
		return (0);
	return (*tmp_1 - *tmp_2);
}

int	ft_strcmp(char *s1, char *s2)
{
	if (!s1 || !s2)
		return (s1 != s2);
	while (*s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}
