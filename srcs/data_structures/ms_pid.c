/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_pid.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 17:15:58 by rgramati          #+#    #+#             */
/*   Updated: 2024/03/10 21:12:53 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_pid	*ms_init_pid(pid_t pid)
{
	t_pid	*new_pid;

	new_pid = malloc(sizeof(t_pid));
	if (!new_pid)
		return (NULL);
	new_pid->pid = pid;
	new_pid->next = NULL;
	return (new_pid);
}

void	ms_pid_push(t_pid **head, t_pid *top)
{
	if (!head || !top)
		return ;
	if (!*head)
	{
		*head = top;
		return ;
	}
	top->next = *head;
	*head = top;
}

t_pid	*ms_pid_pop(t_pid **head)
{
	t_pid	*tmp;

	if (!head || !*head)
		return (NULL);
	tmp = *head;
	*head = (*head)->next;
	tmp->next = NULL;
	return (tmp);
}

void	ms_del_pid_list(t_pid *p_list)
{
	if (!p_list)
		return ;
	ms_del_pid_list(p_list->next);
	free(p_list);
}
