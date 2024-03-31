/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_command.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 15:00:02 by rgramati          #+#    #+#             */
/*   Updated: 2024/03/31 15:50:56 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ms_get_path(char *cmd, t_envvar *envp)
{
	int		i;
	char	*path;
	char	*bin;

	if (!cmd || !*cmd || ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	while (envp && ft_strncmp(envp->name, "PATH", 5))
		envp = envp->next;
	if (!envp)
		return (ft_strjoin("/", ft_strdup(cmd), NULL, 0b10));
	bin = ft_strjoin("/", cmd, 0, 0b00);
	i = 0;
	path = ft_strjoin(envp->values[i++], bin, 0, 0);
	while (envp->values[i] && access(path, X_OK) == -1)
	{
		free(path);
		path = ft_strjoin(envp->values[i++], bin, 0, 0);
	}
	free(bin);
	if (!envp->values[i])
		free(path);
	if (!envp->values[i])
		return (NULL);
	return (path);
}

void	ms_del_command(t_command *cmd)
{
	ms_clear_redir_list(cmd->redirs);
	free(cmd->path);
	ft_free_tab((void **)cmd->args);
	free(cmd);
}
