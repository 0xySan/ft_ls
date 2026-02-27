/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 12:01:42 by etaquet           #+#    #+#             */
/*   Updated: 2025/06/24 18:43:10 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <grp.h>

size_t	getblocksize(t_files *files)
{
	int		i;
	size_t	total_count;

	total_count = 0;
	i = 0;
	while (files->files[i])
	{
		total_count += files->stats[i].st_blocks;
		i++;
	}
	return (total_count / 2);
}

void	getsymlink(struct stat st, char *path)
{
	char	target[PATH_MAX];
	ssize_t	len;

	if (S_ISLNK(st.st_mode))
	{
		len = readlink(path, target, sizeof(target) - 1);
		if (len != -1)
		{
			target[len] = '\0';
			ft_dprintf(1, " -> %s", target);
		}
	}
}

static int	has_user_xattr(const char *path)
{
	char	buf[4096];
	ssize_t	len;
	int		i;

	len = listxattr(path, buf, sizeof(buf));
	if (len <= 0)
		return (0);
	i = 0;
	while (i < len)
	{
		if (strncmp(buf + i, "system.", 7) != 0
			&& strncmp(buf + i, "security.", 9) != 0)
			return (1);
		i += ft_strlen(buf + i) + 1;
	}
	return (0);
}

static void	putchar_perm_special(mode_t mode, int position)
{
	char	c;

	c = '\0';
	if (position == 2)
	{
		if (mode & S_IXUSR)
			c = (mode & S_ISUID) ? 's' : 'x';
		else
			c = (mode & S_ISUID) ? 'S' : '-';
	}
	else if (position == 5)
	{
		if (mode & S_IXGRP)
			c = (mode & S_ISGID) ? 's' : 'x';
		else
			c = (mode & S_ISGID) ? 'S' : '-';
	}
	else if (position == 8)
	{
		if (mode & S_IXOTH)
			c = (mode & S_ISVTX) ? 't' : 'x';
		else
			c = (mode & S_ISVTX) ? 'T' : '-';
	}
	write(1, &c, 1);
}

static void	print_acl_xattr(const char *path)
{
	acl_t	acl;

	acl = acl_get_file(path, ACL_TYPE_ACCESS);
	if (acl)
	{
		if (acl_entries(acl) > 3)
		{
			write(1, "+", 1);
			acl_free(acl);
			return ;
		}
		acl_free(acl);
	}
	if (has_user_xattr(path))
		write(1, "@", 1);
	else
		write(1, " ", 1);
}

static void	update_cw(t_colwidths *cw, struct stat st, int group)
{
	int				tmp;
	struct group	*gr;
	struct passwd	*pw;

	tmp = count_digits((unsigned long)st.st_nlink);
	if (tmp > cw->nlink_w)
		cw->nlink_w = tmp;
	tmp = count_digits((unsigned long)st.st_size);
	if (tmp > cw->size_w)
		cw->size_w = tmp;
	gr = getgrgid(st.st_gid);
	if (gr && ft_strlen(gr->gr_name) > cw->group_w)
		cw->group_w = ft_strlen(gr->gr_name);
	if (!group)
	{
		pw = getpwuid(st.st_uid);
		if (pw && ft_strlen(pw->pw_name) > cw->owner_w)
			cw->owner_w = ft_strlen(pw->pw_name);
	}
}

t_colwidths	init_colwidths(t_files *files, int group, int all)
{
	t_colwidths	cw;
	int			i;

	cw = (t_colwidths){0, 0, 0, 0};
	i = -1;
	while (files->files[++i])
	{
		if (!files->files[i] || !files->real_paths[i])
			continue ;
		if (strncmp(files->files[i], ".", 1) == 0 && !all)
			continue ;
		update_cw(&cw, files->stats[i], group);
	}
	return (cw);
}

void	getperms(struct stat st, const char *path, int group, t_colwidths cw)
{
	struct passwd	*pw;
	struct group	*gr;

	print_file_type(st.st_mode);
	putchar_perm(st.st_mode & S_IRUSR, 'r');
	putchar_perm(st.st_mode & S_IWUSR, 'w');
	putchar_perm_special(st.st_mode, 2);
	putchar_perm(st.st_mode & S_IRGRP, 'r');
	putchar_perm(st.st_mode & S_IWGRP, 'w');
	putchar_perm_special(st.st_mode, 5);
	putchar_perm(st.st_mode & S_IROTH, 'r');
	putchar_perm(st.st_mode & S_IWOTH, 'w');
	putchar_perm_special(st.st_mode, 8);
	print_acl_xattr(path);
	pw = getpwuid(st.st_uid);
	gr = getgrgid(st.st_gid);
	print_padded_num((unsigned long)st.st_nlink, cw.nlink_w);
	write(1, " ", 1);
	if (!group)
	{
		print_padded_str(pw ? pw->pw_name : "?", cw.owner_w);
		write(1, " ", 1);
	}
	print_padded_str(gr ? gr->gr_name : "?", cw.group_w);
	write(1, " ", 1);
	print_padded_num((unsigned long)st.st_size, cw.size_w);
	write(1, " ", 1);
	print_mod_time(st);
}
