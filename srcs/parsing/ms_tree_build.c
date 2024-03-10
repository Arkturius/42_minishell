/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_tree_build.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 16:47:41 by rgramati          #+#    #+#             */
/*   Updated: 2024/03/10 21:32:16 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*ms_cmd_token(t_token **tokens, t_envvar **env, t_token *tmp)
{
	char	**args;
	t_redir	*redirs;

	args = NULL;
	redirs = NULL;
	while (tmp && (tmp->type & (TK_STRING | TK_REDIRS)))
	{
		if (tmp->type & TK_REDIRS)
		{
			ms_add_redir(&redirs, ms_init_redir(tmp));
			tmp = tmp->next;
		}
		else if (tmp->type & TK_STRING)
			ft_strapp(&args, ft_strdup(tmp->str));
		tmp = tmp->next;
	}
	*tokens = tmp;
	return (ms_init_node(ms_init_command(redirs, args, env), NULL));
}

void	ms_brace_tree(t_token **tk, t_node **tree, t_envvar **env)
{
	t_node		*subtree;
	int			level;

	subtree = ms_build_tree((*tk)->next, env);
	level = 0;
	if (!tree)
		*tree = subtree;
	else
		ms_insert_child(tree, subtree, R);
	(*tk) = (*tk)->next;
	while ((*tk) && (!((*tk)->type & TK_BRACES && \
			!ft_strncmp((*tk)->str, ")", 2)) || level))
	{
		if ((*tk)->type & TK_BRACES && !ft_strncmp((*tk)->str, "(", 2))
			level++;
		if ((*tk)->type & TK_BRACES && !ft_strncmp((*tk)->str, ")", 2))
			level--;
		(*tk) = (*tk)->next;
	}
	(*tk) = (*tk)->next;
}

t_node	*ms_extract_pipeline(t_token **tokens, t_envvar **env)
{
	t_node	*tree;

	tree = NULL;
	if (!tokens)
		return (NULL);
	while (*tokens)
	{
		if ((*tokens)->type & (TK_STRING | TK_REDIRS))
		{
			if (!tree)
				tree = ms_cmd_token(tokens, env, *tokens);
			else
				ms_insert_child(&tree, ms_cmd_token(tokens, env, *tokens), R);
		}
		if (*tokens && (*tokens)->type & (TK_BRACES))
		{
			if (!ft_strncmp((*tokens)->str, "(", 2))
				ms_brace_tree(tokens, &tree, env);
		}
		if (!*tokens || !((*tokens)->type & TK_PIPEXS))
			return (tree);
		ms_insert_parent(&tree, ms_init_node(NULL, ms_dup_token(*tokens)), L);
		(*tokens) = (*tokens)->next;
	}
	return (tree);
}

__always_inline
void	ms_tree_mux(t_node **tree, t_token **tokens, t_envvar **env)
{
	t_node	*sub;
	t_token	*new;

	if ((*tokens)->type & TK_BINOPS)
	{
		new = ms_dup_token(*tokens);
		*tokens = (*tokens)->next;
		sub = ms_extract_pipeline(tokens, env);
		ms_associate(tree, sub, NULL, new);
	}
	else if ((*tokens)->type & TK_PIPEXS)
	{
		sub = ms_init_node(NULL, ms_dup_token(*tokens));
		ms_insert_parent(tree, sub, L);
		*tokens = (*tokens)->next;
	}
	if (*tokens && (*tokens)->type & (TK_STRING | TK_REDIRS))
	{
		if (*tree && (*tree)->left)
			ms_insert_child(tree, ms_cmd_token(tokens, env, *tokens), R);
		else
			ms_insert_child(tree, ms_extract_pipeline(tokens, env), L);
	}
}

t_node	*ms_build_tree(t_token *tokens, t_envvar **env)
{
	t_node		*tree;

	tree = NULL;
	ms_update_env(env);
	while (tokens)
	{
		if (tokens->type & TK_BRACES)
		{
			if (!ft_strncmp(tokens->str, "(", 2))
				ms_brace_tree(&tokens, &tree, env);
			else
				return (tree);
		}
		else
			ms_tree_mux(&tree, &tokens, env);
	}
	return (tree);
}
