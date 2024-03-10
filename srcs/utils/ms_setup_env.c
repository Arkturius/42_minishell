/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_setup_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:08:31 by rgramati          #+#    #+#             */
/*   Updated: 2024/03/10 21:36:23 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_envvar	*ms_update_env(t_envvar **env)
{
	static t_envvar	*envlist;

	if (!envlist || env)
		envlist = *env;
	return (envlist);
}

void	ms_create_env(t_envvar **env, char **argv, int i)
{
	char		*tmp;

	if (i)
	{
		tmp = ms_get_pwd();
		ms_set_var(env, "PWD", tmp);
		tmp = ft_strjoin(ms_get_pwd(), argv[0], "/", 0b01);
		ms_set_var(env, "_", tmp);
	}
	tmp = ft_itoa(1);
	ms_set_var(env, "SHLVL", tmp);
}

t_envvar	*ms_setup_env(char **argv, char **envp)
{
	t_envvar	*env;
	char		*tmp;
	int			i;

	i = -1;
	env = NULL;
	tmp = NULL;
	while (envp[++i])
		ms_add_var(&env, ms_init_var(envp[i]));
	ms_create_env(&env, argv, i);
	if (ms_get_var(env, "PWD") && ms_get_var(env, "PWD")->values)
		tmp = ft_strjoin(ms_get_var(env, "PWD")->values[0], "/.logo", "", 0b00);
	ms_set_var(&env, "LOGOP", tmp);
	return (env);
}
