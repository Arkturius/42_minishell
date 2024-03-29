/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parse_wildcards.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 13:31:16 by rgramati          #+#    #+#             */
/*   Updated: 2024/03/28 21:21:18 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ms_regex_wildcard(char *rule, char *file, char *tmp_r)
{
	char	*next;

	while (*file)
	{
		next = NULL;
		while (*tmp_r == '*')
			tmp_r++;
		if (!*tmp_r)
			return (*(tmp_r - 1) == '*');
		if (!ft_strchr(tmp_r, '*'))
			next = ft_strrchr(file, *tmp_r);
		else if (tmp_r != rule || *tmp_r == *file)
			next = ft_strchr(file, *tmp_r);
		if (next)
			file += (next - file);
		else
			return (0);
		while (*file && *tmp_r && *tmp_r != '*' && *tmp_r == *(file++))
			tmp_r++;
	}
	while (*tmp_r && *tmp_r == '*')
		tmp_r++;
	return (!*file && !*tmp_r);
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
			ms_regex_wildcard(wcstr, dentry->d_name, wcstr))
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
