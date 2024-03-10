/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_setup_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:08:31 by rgramati          #+#    #+#             */
/*   Updated: 2024/03/10 20:30:37 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_envvar	*ft_update_env(t_envvar **env)
{
	static t_envvar	*envlist;

	if (!envlist || env)
		envlist = *env;
	return (envlist);
}

void	ft_create_env(t_envvar **env, char **argv, int i)
{
	char		*tmp;

	if (i)
	{
		tmp = ft_get_pwd();
		ft_set_var(env, "PWD", tmp);
		tmp = ft_strjoin(ft_get_pwd(), argv[0], "/", 0b01);
		ft_set_var(env, "_", tmp);
	}
	tmp = ft_itoa(1);
	ft_set_var(env, "SHLVL", tmp);
}

t_envvar	*ft_setup_env(char **argv, char **envp)
{
	t_envvar	*env;
	char		*tmp;
	int			i;

	i = -1;
	env = NULL;
	tmp = NULL;
	while (envp[++i])
		ft_add_var(&env, ft_init_var(envp[i]));
	ft_create_env(&env, argv, i);
	if (ft_get_var(env, "PWD") && ft_get_var(env, "PWD")->values)
		tmp = ft_strjoin(ft_get_var(env, "PWD")->values[0], "/.logo", "", 0b00);
	ft_set_var(&env, "LOGOP", tmp);
	return (env);
}
