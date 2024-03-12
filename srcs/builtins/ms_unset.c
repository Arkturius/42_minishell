/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:07:14 by rgramati          #+#    #+#             */
/*   Updated: 2024/03/11 19:06:34 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ms_unset(t_command *cmd)
{
	char	**tmp;

	tmp = cmd->args + 1;
	while (*tmp)
		ms_remove_var(cmd->envp, *(tmp++));
	return (ERR_NOERRS);
}
