/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 19:10:27 by rgramati          #+#    #+#             */
/*   Updated: 2024/03/25 19:20:15 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_nbr_len(int n)
{
	int	i;

	i = 1;
	if (n < 0)
		n = -n;
	while (n > 10)
	{
		i++;
		n /= 10;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	char	nb[12];
	char	*tmp;
	int		len;

	tmp = (char *) nb;
	while (tmp - (char *) nb < 12)
		*(tmp++) = 0;
	len = ft_nbr_len(n);
	if (!n)
		return (ft_strdup("0"));
	if (n < 0)
	{
		nb[0] = '-';
		n = -n;
		len++;
	}
	while (n)
	{
		nb[--len] = (char){n % 10 + 48};
		n /= 10;
	}
	return (ft_strdup(nb));
}
