/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 12:01:42 by etaquet           #+#    #+#             */
/*   Updated: 2025/06/05 15:25:53 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	ft_write_padded_str(char *str, int width, int padding)
{
	int len = padding - width;
	write(1, str, width);
	while (len > 0)
	{
		write(1, " ", 1);
		len--;
	}
}

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
		write(1, "-", 1);
}

void	print_file_type(mode_t mode)
{
	if ((mode & S_IFMT) == S_IFLNK)
		write(1, "l", 1);
	else if ((mode & S_IFMT) == S_IFDIR)
		write(1, "d", 1);
	else
		write(1, "-", 1);
}

void	print_mod_time(struct stat st)
{
	char		*timestr;

	timestr = ctime(&st.st_mtime);

	write(1, timestr + 4, 12);
    write(1, " ", 1);
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
			write(1, " -> ", 4);
			write(1, target, len);
		}
	}
}

void	getPerms(struct stat st)
{
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
	write(1, " ", 1);
    dprintf(1, "%lu %s %s %lu ", st.st_nlink, getpwuid(st.st_uid)->pw_name, getgrgid(st.st_gid)->gr_name, st.st_size);
    print_mod_time(st);
}