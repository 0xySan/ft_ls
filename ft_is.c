/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 11:09:00 by etaquet           #+#    #+#             */
/*   Updated: 2025/06/24 19:05:46 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int	is_symlink(const char *path)
{
	struct stat	st;

	if (lstat(path, &st) != 0)
		return (0);
	return (S_ISLNK(st.st_mode));
}

int	is_executable_file(const char *path)
{
	struct stat	st;

	if (stat(path, &st) != 0)
		return (0);
	return (S_ISREG(st.st_mode) && access(path, X_OK) == 0);
}

char	*get_real_path(const char *base_path, const char *path)
{
	char	*rp;
	int		blen;
	int		plen;
	int		need_slash;
	int		i;

	blen = ft_strlen(base_path);
	plen = ft_strlen(path);
	need_slash = (blen > 0 && base_path[blen - 1] != '/');
	rp = malloc(blen + need_slash + plen + 1);
	if (!rp)
		return (NULL);
	i = -1;
	while (++i < blen)
		rp[i] = base_path[i];
	if (need_slash)
		rp[i++] = '/';
	blen = -1;
	while (++blen < plen)
		rp[i++] = path[blen];
	rp[i] = '\0';
	return (rp);
}

int	is_directory(const char *path)
{
	struct stat	st;

	if (stat(path, &st) != 0)
		return (0);
	return (S_ISDIR(st.st_mode));
}
