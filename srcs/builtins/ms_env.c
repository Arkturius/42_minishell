/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:05:11 by rgramati          #+#    #+#             */
/*   Updated: 2024/03/10 21:10:17 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ms_var_size(t_envvar *vars)
{
	int	size;

	size = 0;
	while (vars)
	{
		size++;
		vars = vars->next;
	}
	return (size);
}

char	*ms_get_varstring(t_envvar *var, int format, int named)
{
	char	*str;

	str = NULL;
	if (var == NULL)
		return (NULL);
	if (!var->values && named)
		return (ft_strdup(var->name));
	if (named)
		str = ft_strjoin(ft_strdup(var->name), "=", NULL, 0b01);
	if (format)
		str = ft_strjoin(str, "\"", NULL, 1);
	str = ft_strjoin(str, ft_strsjoin(var->values, ":", 0b00), NULL, 0b11);
	if (format)
		str = ft_strjoin(str, "\"", NULL, 0b01);
	return (str);
}

char	**ms_get_var_strs(t_envvar *vars, int format)
{
	int		i;
	char	**vars_array;
	int		size;

	i = -1;
	size = ms_var_size(vars) + 1;
	vars_array = malloc(size * sizeof(char *));
	if (!vars_array)
		return (NULL);
	while (++i < size - 1)
	{
		vars_array[i] = ms_get_varstring(vars, format, 1);
		if (!vars_array[i])
		{
			ft_free_tab((void **)vars_array);
			return (NULL);
		}
		vars = vars->next;
	}
	vars_array[i] = NULL;
	return (vars_array);
}

int	ms_env(t_command *cmd)
{
	t_envvar	*vars;
	char		**vars_array;
	char		**tmp;

	if (ft_tab_len(cmd->args) > 1)
	{
		ms_error_message(ERR_TMARGS, "env");
		return (127);
	}
	vars = *(cmd->envp);
	vars_array = ms_get_var_strs(vars, 0);
	if (!vars_array)
		return (ERR_FAILED);
	tmp = vars_array;
	while (*tmp)
	{
		if (ft_strchr(*tmp, '='))
			ft_dprintf(cmd->outfile, "%s\n", *tmp);
		tmp++;
	}
	ft_free_tab((void **)vars_array);
	return (ERR_NOERRS);
}
