/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_dollars.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 13:19:03 by ycontre           #+#    #+#             */
/*   Updated: 2024/03/10 18:25:23 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_code;

int	ft_var_len(char *str)
{
	char	*tmp;

	tmp = str + 1;
	if (!*tmp)
		return (1);
	if (ft_isdigit(*tmp) || ft_strchr("\"'", *tmp))
		return (2);
	if (!ft_strchr("_?", *tmp) && !ft_isalnum(*tmp))
		return (1);
	if (*tmp == '?')
		return (2);
	tmp++;
	while (*tmp && (ft_isalnum(*tmp) && !ft_strchr("_?", *tmp)))
		tmp++;
	return (tmp - str);
}

void	ft_quoted_skip(char **str, int len, t_qstate *qs)
{
	while (*str && **str && len > 0)
	{
		ft_qs_update(*(*str)++, qs);
		len--;
	}
}

void	ft_insert_var(t_envvar *var_ptr, char ***new, t_qstate qs)
{
	char	*value;

	if (var_ptr)
	{
		value = ft_get_varstring(var_ptr, 0, 0);
		if (qs == QU_ZERO && ft_verify_wildcard(value, QU_ZERO))
			ft_replace_wildcard(&value);
		ft_strapp(new, value);
	}
}

void	ft_manage_vars(t_envvar *vars, char *start, char ***new, t_qstate qs)
{
	int		len;
	char	*name;

	if (*start && *(start + 1) == '?')
	{
		ft_strapp(new, ft_itoa(g_exit_code));
		return ;
	}
	start++;
	if (ft_isdigit(*start))
	{
		if (*start == '0')
			ft_strapp(new, ft_strdup("minishell"));
		return ;
	}
	len = ft_var_len(start - 1);
	if (ft_strchr("\"'", *start))
		return ;
	name = ft_strndup(start, len - 1);
	ft_insert_var(ft_get_var(vars, name), new, qs);
	free(name);
}

char	**ft_replace_vars(t_envvar *vars, char **str, t_qstate qs, int no_q)
{
	char	*tmp;
	char	**new;

	tmp = *str;
	new = NULL;
	if (!tmp)
		return (NULL);
	while (*tmp)
	{
		if (ft_strcspn(tmp, "$") > 0)
			ft_strapp(&new, ft_strndup(tmp, ft_strcspn(tmp, "$")));
		if (no_q && new && *new)
			ft_dequote_string(&(new[ft_tab_len(new) - !!ft_tab_len(new)]), qs);
		ft_quoted_skip(&tmp, ft_strcspn(tmp, "$"), &qs);
		if (!*tmp)
			break ;
		if (qs != QU_SINGLE && ft_var_len(tmp) > 1)
			ft_manage_vars(vars, tmp, &new, qs);
		else
			ft_strapp(&new, ft_strndup(tmp, ft_var_len(tmp)));
		tmp += ft_var_len(tmp);
	}
	free(*str);
	*str = ft_strsjoin(new, NULL, 0b00);
	return (new);
}
