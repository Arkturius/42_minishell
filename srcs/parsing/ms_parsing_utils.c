/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parsing_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 13:19:40 by ycontre           #+#    #+#             */
/*   Updated: 2024/03/12 19:24:51 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ms_qs_update(char c, t_qstate *qs)
{
	t_qstate	tmp;

	tmp = *qs;
	if (*qs == QU_ZERO)
	{
		if (c == '\'')
			*qs = QU_SINGLE;
		if (c == '"')
			*qs = QU_DOUBLE;
	}
	else if ((c == '\'' && *qs == QU_SINGLE) || (c == '"' && *qs == QU_DOUBLE))
		*qs = QU_ZERO;
	return (*qs != tmp);
}

int	ms_dqstrlen(char *str, t_qstate qs)
{
	int			len;

	len = 0;
	while (*str)
	{
		if (!ms_qs_update(*(str++), &qs))
			len++;
	}
	return (len);
}

void	ms_remove_braces(t_token **tokens)
{
	t_token	*tmp;

	tmp = *tokens;
	while (tmp)
	{
		if (!ms_verif_binop_brace(tmp))
		{
			ms_remove_token(&tmp);
			if (!tmp)
			{
				*tokens = NULL;
				continue ;
			}
			if (!tmp->prev)
				*tokens = tmp;
			continue ;
		}
		tmp = tmp->next;
	}
}
