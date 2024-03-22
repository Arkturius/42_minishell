/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 15:01:13 by ycontre           #+#    #+#             */
/*   Updated: 2024/03/22 16:47:25 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_code = 0;

int	main(int argc, char **argv, char **envp)
{
	t_envvar	*env;

	rl_catch_signals = 0;
	rl_outstream = stderr;
	env = ms_setup_env(argv, envp);
	if (argc >= 2)
	{
		ms_error_message(ERR_INVOPT, argv[1]);
		ms_clear_env(env);
		exit(EXIT_FAILURE);
	}
	ms_print_logo(env);
	ms_signal_state(SIGHANDLER_INT);
	while (42)
	{
		ms_update_env(0, &env);
		ms_tree_holder(1, NULL);
		ms_prompt_handler(&env);
	}
	return (EXIT_SUCCESS);
}
