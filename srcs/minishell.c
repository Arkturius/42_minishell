/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 15:01:13 by ycontre           #+#    #+#             */
/*   Updated: 2024/03/31 16:05:01 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_code = 0;

int	ms_launch_single_command(char *line, t_envvar **envp)
{
	int			first;
	t_token		*tokens;
	t_node		*tree;

	tokens = NULL;
	tree = NULL;
	first = 0;
	if (!*line)
	{
		free(line);
		return (g_exit_code);
	}
	if (ms_to_tokens(&tokens, line, envp) || !tokens)
		return (g_exit_code);
	if (ms_to_tree_exec(&tokens, &tree, envp))
		return (g_exit_code);
	ms_close_tree_rec(tree);
	ms_clear_tree(tree);
	ms_clear_env(*envp);
	return (g_exit_code);
}

int	main(int argc, char **argv, char **envp)
{
	t_envvar	*env;

	rl_catch_signals = 0;
	rl_outstream = stderr;
	env = ms_setup_env(argv, envp);
	ms_signal_state(SIGHANDLER_INT);
	if (argc >= 3 && !ft_strncmp(argv[1], "-c", 3))
	{
		g_exit_code = ms_launch_single_command(ft_strdup(argv[2]), &env);
		exit(g_exit_code);
	}
	if (argc >= 2)
	{
		ms_error_message(ERR_INVOPT, argv[1]);
		ms_clear_env(env);
		exit(EXIT_FAILURE);
	}
	ms_print_logo(env);
	while (42)
	{
		ms_update_env(0, &env);
		ms_tree_holder(1, NULL);
		ms_prompt_handler(&env);
	}
	return (EXIT_SUCCESS);
}
