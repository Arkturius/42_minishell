/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_token_auth.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 00:56:54 by marvin            #+#    #+#             */
/*   Updated: 2024/03/12 20:49:25 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ms_sub_valid_token(t_token *t)
{
	t_token_type	bops;
	t_token_type	strs;

	bops = TK_BINOPS | TK_PIPEXS;
	strs = TK_STRING | TK_REDIRS;
	if (t->type == TK_NEWLIN)
		return (1);
	if ((t->type & TK_BINOPS) && (t->next->type & bops))
		return (0);
	else if (t->type == TK_REDIRS && !(t->next->type == TK_STRING))
		return (0);
	else if (t->type == TK_PIPEXS && (t->next->type & bops))
		return (0);
	else if (!ft_strncmp(t->str, "(", 2) && ((t->next->type & bops) || \
			!ft_strncmp(t->next->str, ")", 2)))
		return (0);
	else if (t->type == TK_STRING && (t->next->type & TK_BRACES) && \
			!ft_strncmp(t->next->str, "(", 2))
		return (0);
	else if (!ft_strncmp(t->str, ")", 2) && (t->next->type & strs || \
			!ft_strncmp(t->next->str, "(", 2)))
		return (0);
	return (1);
}

int	ms_valid_token(t_token *t, char **err_token)
{
	if (t->type & (TK_BINOPS | TK_PIPEXS))
		return (0);
	while (t && ms_sub_valid_token(t))
	{
		if (t->next)
			*err_token = t->next->str;
		t = t->next;
	}
	if (!t)
		return (1);
	if (t->type == TK_REDIRS && t->next->type == TK_NEWLIN)
		*err_token = "newline";
	if (t->type == TK_REDIRS && t->next->type != TK_NEWLIN)
		*err_token = t->next->str;
	return (t->type & (TK_BRACES | TK_STRING));
}

int	ms_verify_brace(t_token *tokens)
{
	int	braces;

	braces = 0;
	while (tokens->type != TK_NEWLIN)
	{
		if ((tokens->type & TK_BRACES) && !ft_strncmp(tokens->str, "(", 2))
			braces++;
		else if ((tokens->type & TK_BRACES) && !ft_strncmp(tokens->str, ")", 2))
			braces--;
		if (braces < 0)
			return (0);
		tokens = tokens->next;
	}
	return (braces == 0);
}

int	ms_verify_token(t_token **tokens, char **err_token)
{
	t_token	*tmp;
	int		hdcount;

	ms_add_token(tokens, ms_init_token(ft_strdup("\n"), TK_NEWLIN));
	*err_token = (*tokens)->str;
	if (!ms_valid_token(*tokens, err_token) || !ms_verify_brace(*tokens))
		return (ERR_FAILED);
	tmp = (*tokens);
	hdcount = 0;
	while (tmp->type != TK_NEWLIN)
	{
		hdcount += ((tmp->type & TK_REDIRS) && !ft_strncmp(tmp->str, "<<", 3));
		tmp = tmp->next;
	}
	ms_remove_token(&tmp);
	if (hdcount > 16)
		return (0b100);
	return (ERR_NOERRS);
}

int	ms_verif_binop_brace(t_token *tk)
{
	int		isvalid;
	int		level;

	level = 0;
	isvalid = 0;
	if (!tk || !(tk->type & TK_BRACES && !ft_strncmp(tk->str, "(", 2)))
		return (1);
	tk = tk->next;
	while (tk && (!(tk->type & TK_BRACES && \
		!ft_strncmp(tk->str, ")", 2)) || level))
	{
		if (tk->type & TK_BRACES && !ft_strncmp(tk->str, "(", 2))
			level++;
		if (tk->type & TK_BRACES && !ft_strncmp(tk->str, ")", 2))
			level--;
		if (!level && (tk->type & TK_BINOPS))
			isvalid = 1;
		tk = tk->next;
	}
	if (!tk)
		return (1);
	if (!(tk && isvalid))
		ms_remove_token(&tk);
	return (tk && isvalid);
}
