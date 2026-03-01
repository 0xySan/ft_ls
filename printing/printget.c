/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printget.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:41:33 by etaquet           #+#    #+#             */
/*   Updated: 2026/03/01 03:54:40 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_ls.h"

static void	putchar_perm(int cond, char c)
{
	if (cond)
		buf_write(1, &c, 1);
	else
		buf_write(1, "-", 1);
}

void	print_file_type(mode_t mode)
{
	char	c;

	if ((mode & S_IFMT) == S_IFLNK)
		c = 'l';
	else if ((mode & S_IFMT) == S_IFDIR)
		c = 'd';
	else if ((mode & S_IFMT) == S_IFBLK)
		c = 'b';
	else if ((mode & S_IFMT) == S_IFCHR)
		c = 'c';
	else if ((mode & S_IFMT) == S_IFIFO)
		c = 'p';
	else if ((mode & S_IFMT) == S_IFSOCK)
		c = 's';
	else
		c = '-';
	buf_write(1, &c, 1);
}

void	print_mod_time(struct stat st)
{
	char			*ft;
	char			copy[26];
	static time_t	now;
	static int		year[4];
	int				i;

	if (now == 0)
	{
		time(&now);
		ft = ctime(&now);
		i = -1;
		while (++i < 4)
			year[i] = ft[20 + i];
	}
	ft = ctime(&st.st_mtime);
	i = -1;
	while (++i < 25)
		copy[i] = ft[i];
	copy[25] = '\0';
	if (copy[20] == year[0] && copy[21] == year[1]
		&& copy[22] == year[2] && copy[23] == year[3])
		buf_write(1, copy + 4, 12);
	else
	{
		buf_write(1, copy + 4, 6);
		buf_write(1, "  ", 2);
		buf_write(1, copy + 20, 4);
	}
	buf_write(1, " ", 1);
}
