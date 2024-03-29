/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_tokenizer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 14:55:34 by ycontre           #+#    #+#             */
/*   Updated: 2024/03/12 20:52:00 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ms_is_token(char *str, t_qstate qs)
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
	if (tmp - tokens == 7 && qs == QU_ZERO && *(str - 1) != ' ')
		return (0);
	return (ft_strlen(*tmp));
}

t_token_type	ms_ttyper(char *str, t_qstate qs)
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
	if (!ft_strncmp(str, "2>", 2) || !ft_strncmp(str, ">", 1) \
		|| !ft_strncmp(str, "<", 1))
		return (TK_REDIRS);
	return (TK_STRING);
}

t_token	*ms_tokenizer(char *str, t_qstate qs, int (*tk)(char *, t_qstate))
{
	t_token	*token;
	char	*tstring;
	char	*tmp;
	int		len;

	token = NULL;
	if (!str || !*str)
		return (NULL);
	tmp = str;
	len = tk(tmp, qs);
	while (*tmp && (!len || (ft_isspace(*tmp) && qs != QU_ZERO)))
	{
		ms_qs_update(*(tmp++), &qs);
		len = tk(tmp, qs);
	}
	if (tmp == str)
		tstring = ft_strndup(str, len);
	else
		tstring = ft_strndup(str, tmp - str);
	if (ft_strncmp(tstring, " ", 2))
		token = ms_init_token(ft_strdup(tstring), ms_ttyper(str, qs));
	ms_add_token(&token, ms_tokenizer(str + ft_strlen(tstring), qs, tk));
	free(tstring);
	return (token);
}

int	ms_verify_wildcard(char *str, t_qstate qs)
{
	char	*tmp;

	tmp = str;
	while (*tmp)
	{
		ms_qs_update(*tmp, &qs);
		if (*tmp == '*' && qs == QU_ZERO)
			return (1);
		tmp++;
	}
	return (0);
}

void	ms_tilde_expansion(t_token **tokens, t_envvar *home)
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
			new = ft_strjoin(new, t + 1, ms_get_varstring(home, 0, 0), 0b101);
			free(tmp->str);
			tmp->str = new;
		}
		else
			tmp = tmp->next;
	}
}
