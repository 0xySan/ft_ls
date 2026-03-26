/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printget.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:41:33 by etaquet           #+#    #+#             */
/*   Updated: 2026/03/26 22:25:47 by etaquet          ###   ########.fr       */
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
	time_t			six_months;
	int				i;

	if (now == 0)
		time(&now);
	six_months = 15778476;
	ft = ctime(&st.st_mtime);
	i = -1;
	while (++i < 25)
		copy[i] = ft[i];
	copy[25] = '\0';
	if (st.st_mtime >= now - six_months && st.st_mtime <= now + 3600)
		buf_write(1, copy + 4, 12);
	else
	{
		buf_write(1, copy + 4, 6);
		buf_write(1, "  ", 2);
		buf_write(1, copy + 20, 4);
	}
	buf_write(1, " ", 1);
}
