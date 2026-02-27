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

void	getperms(struct stat st)
{
	struct passwd	*pw;
	struct group	*gr;

	print_file_type(st.st_mode);
	putchar_perm(st.st_mode & S_IRUSR, 'r');
	putchar_perm(st.st_mode & S_IWUSR, 'w');
	putchar_perm(st.st_mode & S_IXUSR, 'x');
	putchar_perm(st.st_mode & S_IRGRP, 'r');
	putchar_perm(st.st_mode & S_IWGRP, 'w');
	putchar_perm(st.st_mode & S_IXGRP, 'x');
	putchar_perm(st.st_mode & S_IROTH, 'r');
	putchar_perm(st.st_mode & S_IWOTH, 'w');
	putchar_perm(st.st_mode & S_IXOTH, 'x');
	pw = getpwuid(st.st_uid);
	gr = getgrgid(st.st_gid);
	ft_dprintf(1, " %u %s %s %u ", st.st_nlink,
		pw ? pw->pw_name : "?",
		gr ? gr->gr_name : "?", st.st_size);
	print_mod_time(st);
}
