/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_token.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 19:10:24 by rgramati          #+#    #+#             */
/*   Updated: 2024/03/10 21:13:39 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_add_token(t_token **tokens, t_token *next)
{
	t_token	*tmp;

	if (!tokens || !next)
		return ;
	if (!(*tokens))
	{
		*tokens = next;
		return ;
	}
	tmp = *tokens;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = next;
	tmp->next->prev = tmp;
}

void	ms_remove_token(t_token **tokens)
{
	t_token	*tmp;

	if ((*tokens)->prev)
	{
		tmp = *tokens;
		(*tokens)->prev->next = tmp->next;
		if (tmp->next)
			tmp->next->prev = (*tokens)->prev;
		*tokens = (*tokens)->prev->next;
		ms_del_token(tmp);
	}
	else
	{
		tmp = *tokens;
		*tokens = tmp->next;
		if (tmp->next)
			(*tokens)->prev = NULL;
		ms_del_token(tmp);
	}
}

t_token	*ms_dup_token(t_token *token)
{
	t_token	*cpy;

	cpy = malloc(sizeof(t_token));
	if (!cpy)
		return (NULL);
	cpy->str = ft_strdup(token->str);
	cpy->type = token->type;
	cpy->next = NULL;
	cpy->prev = NULL;
	return (cpy);
}

void	ms_del_token(t_token *token)
{
	if (!token)
		return ;
	free(token->str);
	free(token);
}

void	ms_clear_token_list(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens->next;
		ms_del_token(tokens);
		tokens = tmp;
	}
}
