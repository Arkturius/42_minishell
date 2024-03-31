/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parse_dollars.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 13:19:03 by ycontre           #+#    #+#             */
/*   Updated: 2024/03/30 17:39:59 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_code;

int	ms_var_len(char *str)
{
	char	*tmp;

	tmp = str + 1;
	if (!*tmp)
		return (1);
	if (ft_isdigit(*tmp) || *tmp == '?')
		return (2);
	if (!ft_isalnum(*tmp) || *tmp == '"')
		return (1);
	tmp++;
	while (*tmp && ((ft_isalnum(*tmp) && *tmp != '?') || *tmp == '_'))
		tmp++;
	return (tmp - str);
}

void	ms_quoted_skip(char **str, int len, t_qstate *qs)
{
	while (*str && **str && len > 0)
	{
		ms_qs_update(*(*str)++, qs);
		len--;
	}
}

void	ms_insert_var(t_envvar *var_ptr, char ***new, t_qstate qs)
{
	char	*value;
	char	**split;

	if (var_ptr)
	{
		value = ms_get_varstring(var_ptr, 0, 0);
		if (qs == QU_ZERO)
			ms_replace_wildcard(&value);
		if (qs == QU_ZERO)
		{
			split = ft_split(value, ' ');
			free(value);
			if (split)
				value = ft_strsjoin(split, "\026", 0b01);
			else
				value = NULL;
		}
		ft_strapp(new, value);
	}
}

void	ms_manage_vars(t_envvar *vars, char *start, char ***new, t_qstate qs)
{
	int		len;
	char	*name;

	if (*start && *(start + 1) == '?')
		ft_strapp(new, ft_itoa(g_exit_code));
	if (*start && *(start + 1) == '?')
		return ;
	start++;
	if (ft_isdigit(*start))
	{
		if (*start == '0')
			ft_strapp(new, ft_strdup("minishell"));
		return ;
	}
	len = ms_var_len(start - 1);
	if (ft_strchr("\"'", *start))
	{
		if (qs == QU_ZERO)
			return ;
		ft_strapp(new, ft_strdup("$"));
		return ;
	}
	name = ft_strndup(start, len - 1);
	ms_insert_var(ms_get_var(vars, name), new, qs);
	free(name);
}

void	ms_replace_vars(t_envvar *vars, char **str, t_qstate qs, int no_q)
{
	char	*tmp;
	char	**new;

	tmp = *str;
	new = NULL;
	if (!tmp)
		return ;
	while (*tmp)
	{
		if (ft_strcspn(tmp, "$") > 0)
			ft_strapp(&new, ft_strndup(tmp, ft_strcspn(tmp, "$")));
		if (no_q && new && *new)
			ms_dequote_string(&(new[ft_tab_len(new) - !!ft_tab_len(new)]), qs);
		ms_quoted_skip(&tmp, ft_strcspn(tmp, "$"), &qs);
		if (!*tmp)
			break ;
		if (qs != QU_SINGLE && ms_var_len(tmp) > 1)
			ms_manage_vars(vars, tmp, &new, qs);
		else
			ft_strapp(&new, ft_strndup(tmp, ms_var_len(tmp)));
		tmp += ms_var_len(tmp);
	}
	free(*str);
	*str = ft_strsjoin(new, NULL, 0b01);
}
