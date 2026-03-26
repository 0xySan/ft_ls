/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hyperlink.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 20:48:16 by etaquet           #+#    #+#             */
/*   Updated: 2026/03/26 20:51:23 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

char	*build_hyperlink_path(const char *raw_path)
{
	char		*hyperlink;
	char		cwd[4096];
	char		*tmp;
	const char	*path;

	path = raw_path;
	hyperlink = realpath(path, NULL);
	if (!hyperlink && path[0] != '/' && getcwd(cwd, sizeof(cwd)))
	{
		if (ft_strncmp(path, "./", 2) == 0)
			path += 2;
		tmp = get_real_path(cwd, path);
		if (tmp)
			hyperlink = tmp;
	}
	if (!hyperlink)
		hyperlink = ft_strdup(path);
	return (hyperlink);
}

void	print_name_hyperlink(const char *name, struct stat *st, t_flags *flags,
	const char *path)
{
	print_name_with_hyperlink(name, path, st, flags);
}

void	write_hyperlink_open(const char *hyperlink)
{
	char	host[256];

	if (!hyperlink)
		return ;
	buf_write(1, "\e]8;;file://", 12);
	if (gethostname(host, sizeof(host)) == 0)
	{
		host[sizeof(host) - 1] = '\0';
		buf_write(1, host, ft_strlen(host));
	}
	buf_write(1, hyperlink, ft_strlen(hyperlink));
	buf_write(1, "\e\\", 2);
}

void	write_hyperlink_close(void)
{
	buf_write(1, "\e]8;;\e\\", 7);
}

void	print_normal_name(const char *name, struct stat *st, t_flags *flags)
{
	if (flags->color && st)
		print_color_name(name, st, flags->color);
	else
		ft_dprintf(1, "%s", name);
}

void	print_name_with_hyperlink(const char *name, const char *path,
	struct stat *st, t_flags *flags)
{
	char	*hyperlink;

	if (!flags->hyperlink || !path)
	{
		print_normal_name(name, st, flags);
		return ;
	}
	hyperlink = build_hyperlink_path(path);
	if (!hyperlink)
	{
		print_normal_name(name, st, flags);
		return ;
	}
	write_hyperlink_open(hyperlink);
	print_normal_name(name, st, flags);
	write_hyperlink_close();
	free(hyperlink);
}

void	print_symlink_with_hyperlink(struct stat st, const char *path,
	int color, t_flags *flags, const char *hyperlink)
{
	char		target[4096];
	ssize_t		len;
	struct stat	st_target;
	const char	*cc;

	if (!S_ISLNK(st.st_mode))
		return ;
	len = readlink(path, target, sizeof(target) - 1);
	if (len <= 0 || len >= (ssize_t)sizeof(target))
		return ;
	target[len] = '\0';
	buf_write(1, " -> ", 4);
	if (flags->hyperlink && hyperlink)
		write_hyperlink_open(hyperlink);
	cc = NULL;
	if (color && stat(path, &st_target) == 0)
		cc = get_color_code(&st_target);
	if (cc)
	{
		buf_write(1, cc, ft_strlen(cc));
		buf_write(1, target, len);
		buf_write(1, "\033[0m", 4);
	}
	else
		buf_write(1, target, len);
	if (flags->hyperlink && hyperlink)
		write_hyperlink_close();
}