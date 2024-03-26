/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_close_v.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 18:46:59 by rgramati          #+#    #+#             */
/*   Updated: 2024/03/26 14:51:45 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_close_v(int nb, ...)
{
	va_list	files;
	int		curr;

	va_start(files, nb);
	while (nb--)
	{
		curr = va_arg(files, int);
		if (curr >= 0)
			close(curr);
	}
	va_end(files);
}
