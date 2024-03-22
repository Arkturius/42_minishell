/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_setup_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:08:31 by rgramati          #+#    #+#             */
/*   Updated: 2024/03/22 16:50:36 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_envvar	*ms_update_env(int reset, t_envvar **env)
{
	static t_envvar	*envlist = NULL;

	if (!envlist || env)
	{
		if (env)
			envlist = *env;
	}
	if (reset)
		envlist = NULL;
	return (envlist);
}

void	ms_create_env(t_envvar **env, char **argv)
{
	char		*tmp;
	t_envvar	*tmp_env;

	tmp = ms_get_pwd();
	ms_set_var(env, "PWD", tmp);
	tmp_env = ms_get_var(*env, "SHLVL");
	if (tmp_env && tmp_env->values)
		tmp = ft_itoa(ft_atoi(tmp_env->values[0]) + 1);
	else
		tmp = ft_strdup("1");
	ms_set_var(env, "SHLVL", tmp);
	ms_set_var(env, "_", ft_strdup(argv[0]));
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
	ms_create_env(&env, argv);
	tmp = ft_strjoin(ms_get_pwd(), "/.logo", "", 0b01);
	ms_set_var(&env, "LOGOP", tmp);
	return (env);
}
