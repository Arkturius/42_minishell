/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 14:56:29 by rgramati          #+#    #+#             */
/*   Updated: 2024/03/10 18:06:40 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(char *str, int c)
{
	while (*str && *str != (unsigned char)c)
		str++;
	if (!*str && (unsigned char)c)
		return (NULL);
	return (str);
}
