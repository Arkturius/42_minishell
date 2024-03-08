/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tree_build.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 16:47:41 by rgramati          #+#    #+#             */
/*   Updated: 2024/03/08 23:05:23 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*ft_cmd_token(t_token **tokens, t_envvar **env, t_token *tmp)
{
	char	**args;
	t_redir	*redirs;

	args = NULL;
	redirs = NULL;
	while (tmp && (tmp->type & (TK_STRING | TK_REDIRS)))
	{
		if (tmp->type & TK_REDIRS)
		{
			ft_add_redir(&redirs, ft_init_redir(tmp));
			tmp = tmp->next;
		}
		else if (tmp->type & TK_STRING)
			ft_strapp(&args, ft_strdup(tmp->str));
		tmp = tmp->next;
	}
	*tokens = tmp;
	return (ft_init_node(ft_init_command(redirs, args, env), NULL));
}

void	ft_brace_tree(t_token **tk, t_node **tree, t_envvar **env)
{
	t_node		*subtree;
	int			level;

	subtree = ft_build_tree((*tk)->next, env);
	level = 0;
	if (!tree)
		*tree = subtree;
	else
		ft_insert_child(tree, subtree, RIGHT);
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

t_node	*ft_extract_pipeline(t_token **tokens, t_envvar **env)
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
				tree = ft_cmd_token(tokens, env, *tokens);
			else
				ft_insert_child(&tree, ft_cmd_token(tokens, env, *tokens), RIGHT);
		}
		if (*tokens && (*tokens)->type & (TK_BRACES))
		{
			if (!ft_strncmp((*tokens)->str, "(", 2))
				ft_brace_tree(tokens, &tree, env);
		}
		if (!*tokens || !((*tokens)->type & TK_PIPEXS))
			return (tree);
		ft_insert_parent(&tree, ft_init_node(NULL, ft_dup_token(*tokens)), LEFT);
		(*tokens) = (*tokens)->next;
	}
	return (tree);
}

t_node	*ft_build_tree(t_token *tokens, t_envvar **env)
{
	t_node		*tree;
	
	tree = NULL;
	ft_update_env(env);
	while (tokens)
	{
		if (tokens->type & TK_BRACES)
		{
			if (!ft_strncmp(tokens->str, "(", 2))
				ft_brace_tree(&tokens, &tree, env);
			else
				return (tree);
		}
		else if (tokens->type & TK_BINOPS)
		{
			t_token *newtok = ft_dup_token(tokens);
			tokens = tokens->next;
			ft_associate(&tree, ft_extract_pipeline(&tokens, env), NULL, newtok);
		}
		else if (tokens->type & TK_PIPEXS)
		{
			ft_insert_parent(&tree, ft_init_node(NULL, ft_dup_token(tokens)), LEFT);
			tokens = tokens->next;
		}
		if (tokens && tokens->type & (TK_STRING | TK_REDIRS))
		{
			if (tree && tree->left)
				ft_insert_child(&tree, ft_cmd_token(&tokens, env, tokens), RIGHT);
			else
				ft_insert_child(&tree, ft_extract_pipeline(&tokens, env), LEFT);
		}
	}
	return (tree);
}
