/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_close_v.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 18:46:59 by rgramati          #+#    #+#             */
/*   Updated: 2024/03/06 19:20:09 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_close_v(int nb, int fd, ...)
{
	va_list	files;
	int		i;
	int		curr;

	i = 0;
	close(fd);
	va_start(files, fd);
	while (i++ < nb)
	{
		curr = va_arg(files, int);
		if (curr >= 0)
			close(curr);
	}
	va_end(files);
}
