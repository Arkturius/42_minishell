/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parse_wildcards.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 13:31:16 by rgramati          #+#    #+#             */
/*   Updated: 2024/03/31 14:49:55 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ms_regex_wildcard(char *rule, char *file, int toskip)
{
	char	*len;

	if (!rule || !file)
		return (0);
	if (!*rule && !*file)
		return (1);
	if (*rule == '*')
		return (ms_regex_wildcard(rule + 1, file, 1));
	if (!*file && *rule)
		return (0);
	if (!ft_strchr(rule, '*'))
		return (!ft_strrncmp(rule, file, ft_strlen(rule)));
	if (*rule == *file)
		return (ms_regex_wildcard(rule + 1, file + 1, 0));
	if (toskip)
	{
		len = ft_strchr(file, *rule);
		if (len)
			return (ms_regex_wildcard(rule, len, 0));
	}
	return (0);
}

char	**ms_wildcard_array(char *wcstr)
{
	char			*dir;
	char			**files;
	DIR				*cdir;
	struct dirent	*dentry;

	dir = ms_get_pwd();
	cdir = opendir(dir);
	if (!cdir)
	{
		ms_error_message(ERR_NOFORD, dir);
		free(dir);
		return (NULL);
	}
	free(dir);
	dentry = readdir(cdir);
	files = NULL;
	while (dentry)
	{
		if (*(dentry->d_name) != '.' && \
			ms_regex_wildcard(wcstr, dentry->d_name, 0))
			ft_strapp(&files, ft_strdup(dentry->d_name));
		dentry = readdir(cdir);
	}
	closedir(cdir);
	return (files);
}

char	*ms_format_wildcard(char ***files)
{
	char	*formatted;

	formatted = NULL;
	ft_sort_lowstrs_tab(*files, ft_tab_len(*files));
	return (ft_strsjoin(*files, ft_strdup("\026"), 0b10));
}

void	ms_replace_wildcard(char **str)
{
	char	**files;
	char	*wcs;
	char	*deq;

	deq = ft_strdup(*str);
	ms_dequote_string(&deq, QU_ZERO);
	if (ms_verify_wildcard(*str, QU_ZERO))
	{
		files = ms_wildcard_array(deq);
		free(deq);
		if (files && *files)
		{
			free(*str);
			wcs = ms_format_wildcard(&files);
			*str = wcs;
		}
		ft_free_tab((void **)(files));
	}
	else
	{
		free(*str);
		*str = deq;
	}
}
