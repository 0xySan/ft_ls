/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 11:09:00 by etaquet           #+#    #+#             */
/*   Updated: 2025/06/04 12:03:48 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <pwd.h>

int is_symlink(const char *path)
{
	struct stat st;

	if (lstat(path, &st) != 0)
		return (0);
	return (S_ISLNK(st.st_mode));
}

int is_executable_file(const char *path)
{
	struct stat st;

	if (stat(path, &st) != 0)
		return (0);
	return (S_ISREG(st.st_mode) && access(path, X_OK) == 0);
}

char *get_real_path(const char *base_path, const char *path)
{
	char	*real_path;

	real_path = malloc(sizeof(char) * (strlen(base_path) + strlen(path) + 2));
	strcpy(real_path, base_path);
	if (real_path[strlen(real_path) - 1] != '/')
		strcat(real_path, "/");
	strcat(real_path, path);
	return (real_path);
}

int	is_directory(const char *base_path, const char *path)
{
	struct stat	st;
	char		*real_path;

	if (!path)
		return 0;
	if (base_path == NULL)
	{
		if (stat(path, &st) != 0)
			return 0;
		return S_ISDIR(st.st_mode);
	}
	else
	{
		real_path = malloc(sizeof(char) * (strlen(base_path) + strlen(path) + 2));
		strcpy(real_path, base_path);
		if (real_path[strlen(real_path) - 1] != '/')
			strcat(real_path, "/");
		strcat(real_path, path);
		if (stat(real_path, &st) != 0)
			return (free(real_path), 0);
		free(real_path);
		return S_ISDIR(st.st_mode);
	}
}