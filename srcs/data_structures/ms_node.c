/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_node.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 18:28:17 by rgramati          #+#    #+#             */
/*   Updated: 2024/03/10 21:12:39 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_insert_parent(t_node **tree, t_node *root, int side)
{
	if (!tree || !root)
		return ;
	if (!(*tree))
	{
		*tree = root;
		return ;
	}
	if (side == L)
		root->left = *tree;
	else
		root->right = *tree;
	*tree = root;
}

void	ms_insert_child(t_node **tree, t_node *child, int side)
{
	if (!tree)
		return ;
	if (!(*tree))
	{
		*tree = child;
		return ;
	}
	if (side == L)
		(*tree)->left = child;
	else
		(*tree)->right = child;
}

void	ms_associate(t_node **tree, t_node *neigh, t_command *c, t_token *t)
{
	if (!tree || !neigh)
		return ;
	if (!(*tree))
	{
		*tree = neigh;
		return ;
	}
	ms_insert_parent(tree, ms_init_node(c, t), L);
	ms_insert_child(tree, neigh, R);
}

void	ms_del_node(t_node *tree)
{
	if (!tree)
		return ;
	if (tree->token)
		ms_del_token(tree->token);
	if (tree->command)
		ms_del_command(tree->command);
	free(tree);
}

void	ms_clear_tree(t_node *tree)
{
	if (!tree)
		return ;
	ms_clear_tree(tree->left);
	ms_clear_tree(tree->right);
	if (tree->token)
		ms_del_token(tree->token);
	if (tree->command)
		ms_del_command(tree->command);
	free(tree);
}
