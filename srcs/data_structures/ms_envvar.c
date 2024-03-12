/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_envvar.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 22:33:30 by rgramati          #+#    #+#             */
/*   Updated: 2024/03/12 20:52:17 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_add_var(t_envvar **vars, t_envvar *var)
{
	t_envvar	*tmp;

	if (!vars || !var)
		return ;
	if (!(*vars))
	{
		*vars = var;
		return ;
	}
	tmp = *vars;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = var;
}

void	ms_remove_var(t_envvar **vars, char *name)
{
	t_envvar	*prv;
	t_envvar	*tmp;
	t_envvar	*head;

	tmp = *vars;
	prv = tmp;
	head = *vars;
	while (tmp && ft_strcmp(tmp->name, name))
	{
		prv = tmp;
		tmp = tmp->next;
	}
	if (!tmp || prv == tmp)
		return ;
	if (prv == tmp)
		head = tmp->next;
	else
		prv->next = tmp->next;
	ms_del_var(tmp);
	*vars = head;
	ms_update_env(!*vars, vars);
}

void	ms_del_var(t_envvar *var)
{
	if (!var)
		return ;
	free(var->name);
	if (var->values && *(var->values))
		ft_free_tab((void **) var->values);
	else
		free(var->values);
	free(var);
	var = NULL;
}

void	ms_clear_env(t_envvar *vars)
{
	t_envvar	*tmp;

	while (vars)
	{
		tmp = vars->next;
		ms_del_var(vars);
		vars = tmp;
	}
}
