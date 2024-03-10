/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 18:16:41 by rgramati          #+#    #+#             */
/*   Updated: 2024/01/24 18:16:41 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_code;

t_error	ms_prompt_line(char **line, t_envvar **envp)
{
	char	*prompt;
	int		err_code;

	prompt = ms_get_prompt_string(*envp);
	*line = readline(prompt);
	free(prompt);
	if (*line && !ms_isnt_empty(*line))
	{
		free(*line);
		return (ERR_FAILED);
	}
	ms_signal_state(SIGHANDLER_IGN);
	err_code = ms_quote_handler(line, envp, 0);
	ms_signal_state(SIGHANDLER_INT);
	if (err_code == ERR_ERRORS)
	{
		ms_clear_env(*envp);
		ms_exit(NULL);
	}
	else if (err_code == ERR_FAILED || !*line)
	{
		g_exit_code = 130;
		return (ERR_DQSTOP);
	}
	return (ERR_NOERRS);
}

t_error	ms_to_tokens(t_token **tokens, char *line, t_envvar **envp)
{
	t_error	syntax;
	char	*err_token;

	syntax = (!!ms_quote_error(line, NULL, QU_ZERO) << 1);
	*tokens = ms_tokenizer(line, QU_ZERO);
	syntax |= ms_verify_token(*tokens, &err_token);
	if (syntax & 0b011)
	{
		if (syntax & 0b10)
			ms_error_message(ERR_SYNTXQ, ": unexcepted end of file");
		else
			ms_error_message(ERR_SYNTXT, err_token);
		ms_clear_token_list(*tokens);
		free(line);
		g_exit_code = 2;
		return (ERR_FAILED);
	}
	ms_tilde_expansion(tokens, ms_get_var(*envp, "HOME"));
	ms_remove_braces(tokens);
	if (!*tokens)
		return (ERR_FAILED);
	free(line);
	if (syntax & 0b100)
		ms_heredoc_limit(*tokens, envp);
	return (ERR_NOERRS);
}

t_error	ms_to_tree_exec(t_token **tokens, t_node **tree, t_envvar **envp)
{
	t_executer	*exe;
	t_pid		*towait;
	int			err_code;
	t_fd		base_fd;

	*tree = ms_build_tree(*tokens, envp);
	ms_clear_token_list(*tokens);
	ms_tree_holder(0, *tree);
	if (ms_heredoc_opening(*tree))
		return (ERR_HDSTOP);
	exe = ms_init_executer();
	ms_signal_state(SIGHANDLER_IGN);
	base_fd = (t_fd){0, 1};
	ms_exec_mux(*tree, base_fd, exe, EX_LWAIT);
	while (exe->pids)
	{
		towait = ms_pid_pop(&(exe->pids));
		waitpid(towait->pid, &err_code, 0);
		g_exit_code = WEXITSTATUS(err_code);
		ms_command_exit(err_code);
		free(towait);
	}
	ms_signal_state(SIGHANDLER_INT);
	free(exe);
	return (ERR_NOERRS);
}

t_error	ms_heredoc_opening(t_node *tree)
{
	int	hd_done;

	hd_done = 0;
	if (ms_manage_heredocs(tree, &hd_done) == ERR_HDSTOP)
	{
		ms_close_tree_rec(tree);
		ms_clear_tree(tree);
		return (ERR_FAILED);
	}
	return (ERR_NOERRS);
}

void	ms_prompt_handler(t_envvar **envp)
{
	char		*line;
	int			first;
	t_token		*tokens;
	t_node		*tree;

	line = NULL;
	tokens = NULL;
	tree = NULL;
	first = 0;
	if (ms_prompt_line(&line, envp))
		return ;
	if (ms_to_tokens(&tokens, line, envp) || !tokens)
		return ;
	if (ms_to_tree_exec(&tokens, &tree, envp))
		return ;
	ms_close_tree_rec(tree);
	ms_clear_tree(tree);
}
