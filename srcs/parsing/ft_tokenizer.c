/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenizer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 14:55:34 by ycontre           #+#    #+#             */
/*   Updated: 2024/03/10 20:01:07 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_is_token(char *str, t_qstate qs)
{
	char		**tmp;
	static char	*tokens[13] = {" ", ";", "(", ")", "||", "&&", \
								"|", "2>", ">>", ">", "<<", "<", NULL};

	tmp = tokens;
	if (!str || !*str)
		return (0);
	if (ft_isspace(*str) && qs == QU_ZERO)
		return (1);
	while (*tmp && (ft_strncmp(str, *tmp, ft_strlen(*tmp)) || qs != QU_ZERO))
		tmp++;
	return (ft_strlen(*tmp));
}

t_token_type	ft_ttyper(char *str, t_qstate qs)
{
	if (qs != QU_ZERO)
		return (TK_STRING);
	if (!ft_strncmp(str, "(", 1) || !ft_strncmp(str, ")", 1))
		return (TK_BRACES);
	if (!ft_strncmp(str, "||", 2) || !ft_strncmp(str, "&&", 2) \
		|| !ft_strncmp(str, ";", 1))
		return (TK_BINOPS);
	if (!ft_strncmp(str, "|", 1))
		return (TK_PIPEXS);
	if (!ft_strncmp(str, ">", 1) || !ft_strncmp(str, "<", 1) \
		|| !ft_strncmp(str, "2>", 2))
		return (TK_REDIRS);
	return (TK_STRING);
}

t_token	*ft_tokenizer(char *str, t_qstate qs)
{
	t_token	*token;
	char	*tstring;
	char	*tmp;
	int		len;

	token = NULL;
	if (!str || !*str)
		return (NULL);
	tmp = str;
	len = ft_is_token(tmp, qs);
	while (*tmp && (!len || (ft_isspace(*tmp) && qs != QU_ZERO)))
	{
		ft_qs_update(*(tmp++), &qs);
		len = ft_is_token(tmp, qs);
	}
	if (tmp == str)
		tstring = ft_strndup(str, len);
	else
		tstring = ft_strndup(str, tmp - str);
	if (ft_strncmp(tstring, " ", 2))
		token = ft_init_token(ft_strdup(tstring), ft_ttyper(str, qs));
	ft_add_token(&token, ft_tokenizer(str + ft_strlen(tstring), qs));
	free(tstring);
	return (token);
}

int	ft_verify_wildcard(char *str, t_qstate qs)
{
	char	*tmp;

	tmp = str;
	while (*tmp)
	{
		ft_qs_update(*tmp, &qs);
		if (*tmp == '*' && qs == QU_ZERO)
			return (1);
		tmp++;
	}
	return (0);
}

void	ft_tilde_expansion(t_token **tokens, t_envvar *home)
{
	t_token		*tmp;
	char		*t;
	char		*new;

	tmp = *tokens;
	while (tmp)
	{
		t = ft_strchr(tmp->str, '~');
		if (t && t == tmp->str && ((*(t + 1) == '/') ^ !*(t + 1)) && home)
		{
			new = ft_strndup(tmp->str, t - tmp->str);
			new = ft_strjoin(new, t + 1, ft_get_varstring(home, 0, 0), 0b101);
			free(tmp->str);
			tmp->str = new;
		}
		else
			tmp = tmp->next;
	}
}
