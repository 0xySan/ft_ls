/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 12:01:42 by etaquet           #+#    #+#             */
/*   Updated: 2025/06/05 17:40:42 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

size_t getBlockSize(t_files *files)
{
    int i;
    size_t total_count;

    total_count = 0;
    i = 0;
    while (files->files[i])
    {
        total_count += files->stats[i].st_blocks;
        i++;
    }
    return total_count / 2;
}

void	putchar_perm(int cond, char c)
{
	if (cond)
		write(1, &c, 1);
	else
		ft_dprintf(1, "-");
}

void	print_file_type(mode_t mode)
{
	if ((mode & S_IFMT) == S_IFLNK)
		ft_dprintf(1, "l");
	else if ((mode & S_IFMT) == S_IFDIR)
		ft_dprintf(1, "d");
	else
		ft_dprintf(1, "-");
}

void	print_mod_time(struct stat st)
{
	char		*timestr;

	timestr = ctime(&st.st_mtime);

	write(1, timestr + 4, 12);
    ft_dprintf(1, " ");
}

void getSymlink(struct stat st, char *path)
{
	if (S_ISLNK(st.st_mode))
	{
		char target[PATH_MAX];
		ssize_t len = readlink(path, target, sizeof(target) - 1);
		if (len != -1)
		{
			target[len] = '\0';
			ft_dprintf(1, " -> %s", target);
		}
	}
}

void	getPerms(struct stat st)
{
	static t_cache cache = { .last_uid = (uid_t)-1, .last_gid = (gid_t)-1, .pw_name = NULL, .gr_name = NULL };
	struct passwd *pw = NULL;
	struct group  *gr = NULL;

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

	if (cache.last_uid != st.st_uid) {
		pw = getpwuid(st.st_uid);
		cache.last_uid = st.st_uid;
		cache.pw_name = pw ? pw->pw_name : "UNKNOWN";
	}
	if (cache.last_gid != st.st_gid) {
		gr = getgrgid(st.st_gid);
		cache.last_gid = st.st_gid;
		cache.gr_name = gr ? gr->gr_name : "UNKNOWN";
	}

	ft_dprintf(1, " %u %s %s %u ", st.st_nlink, cache.pw_name, cache.gr_name, st.st_size);
	print_mod_time(st);
}