/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ptrdisplays.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 14:22:46 by rgramati          #+#    #+#             */
/*   Updated: 2024/03/08 15:44:22 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_putstrtab_fd(int fd, char **tab)
{
	size_t	len;
	char	**tmp;

	tmp = tab;
	len = ft_putstr_fd(fd, "{");
	while (tmp && *tmp)
	{
		if (tmp != tab)
			len += ft_putstr_fd(fd, ", ");
		len += ft_putstr_fd(fd, *(tmp++));
	}
	ft_putstr_fd(2, ", NULL");
	len += ft_putstr_fd(2, "}");
	return (len);
}
