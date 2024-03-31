/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 16:43:39 by rgramati          #+#    #+#             */
/*   Updated: 2024/03/31 15:58:35 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ms_isnt_empty(char *str)
{
	char	*tmp;

	if (!*str)
		return (0);
	tmp = str;
	while (ft_isspace(*tmp))
		tmp++;
	return (*tmp);
}

void	ms_print_logo(t_envvar *envp)
{
	int		fd;
	char	*line;

	if (!ms_get_var(envp, "LOGOP"))
		return ;
	fd = open(ms_get_var(envp, "LOGOP")->values[0], 0);
	if (fd < 0)
		return ;
	line = get_next_line(fd);
	ft_printf("\033c\033[31m");
	while (line)
	{
		usleep(20000);
		ft_printf("%s", line);
		free(line);
		line = get_next_line(fd);
	}
	ft_printf("\033[34m\n");
	close(fd);
}

char	*ms_get_temp_file(char *head, int size)
{
	int		urandom;
	char	*rand;
	char	*tmp;

	rand = ft_calloc((size + 1), sizeof(char));
	urandom = open("/dev/urandom", O_RDONLY);
	if (urandom < 0)
		return (NULL);
	read(urandom, rand, size);
	close(urandom);
	tmp = rand;
	while (size--)
	{
		while (!(ft_isalnum(*rand)))
			*rand = (unsigned char) *rand % 93 + 33;
		rand++;
	}
	return (ft_strjoin(head, tmp, "-", 0b10));
}

void	ms_connect_input(t_command *cmd, int hd_last)
{
	if (hd_last)
	{
		if (cmd->infile != STDIN_FILENO)
			close(cmd->infile);
		cmd->infile = cmd->heredoc;
	}
	else
	{
		if (cmd->heredoc > 2)
			close(cmd->heredoc);
	}
}
