/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:34:03 by etaquet           #+#    #+#             */
/*   Updated: 2025/06/24 18:44:55 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

bool	file_exists(char *filename)
{
	struct stat	buffer;

	return (stat(filename, &buffer) == 0);
}

int	count_files(char **av)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (av[i])
	{
		if (av[i][0] && av[i][0] != '-' && file_exists(av[i]))
			count++;
		i++;
	}
	return (count);
}

int	count_digits(unsigned long n)
{
	int	count;

	count = 1;
	while (n >= 10)
	{
		n /= 10;
		count++;
	}
	return (count);
}

void	print_padded_num(unsigned long n, int width)
{
	int			pad;
	static char	spaces[32] = "                                ";

	pad = width - count_digits(n);
	if (pad > 0 && pad <= 32)
		buf_write(1, spaces, pad);
	ft_putunit_max(1, n, 10);
}

void	print_padded_str(const char *s, int width)
{
	int			len;
	int			pad;
	static char	spaces[64] = "                                "
		"                                ";

	len = ft_strlen(s);
	buf_write(1, s, len);
	pad = width - len;
	while (pad > 0)
	{
		if (pad > 64)
		{
			buf_write(1, spaces, 64);
			pad -= 64;
		}
		else
		{
			buf_write(1, spaces, pad);
			pad = 0;
		}
	}
}

static const char	*get_color_dir(mode_t m)
{
	if ((m & S_ISVTX) && (m & S_IWOTH))
		return ("\033[30;42m");
	if (m & S_IWOTH)
		return ("\033[34;42m");
	if (m & S_ISVTX)
		return ("\033[37;44m");
	return ("\033[01;34m");
}

const char	*get_color_code(struct stat *st)
{
	mode_t	m;

	m = st->st_mode;
	if (S_ISDIR(m))
		return (get_color_dir(m));
	if (S_ISLNK(m))
		return ("\033[01;36m");
	if (S_ISFIFO(m))
		return ("\033[40;33m");
	if (S_ISSOCK(m))
		return ("\033[01;35m");
	if (S_ISBLK(m))
		return ("\033[40;33;01m");
	if (S_ISCHR(m))
		return ("\033[40;33;01m");
	if (m & S_ISUID)
		return ("\033[37;41m");
	if (m & S_ISGID)
		return ("\033[30;43m");
	if (m & (S_IXUSR | S_IXGRP | S_IXOTH))
		return ("\033[01;32m");
	return (NULL);
}

void	print_color_name(const char *name, struct stat *st, int color)
{
	const char	*cc;

	cc = NULL;
	if (color && st)
		cc = get_color_code(st);
	if (cc)
	{
		buf_write(1, cc, ft_strlen(cc));
		buf_write(1, name, ft_strlen(name));
		buf_write(1, "\033[0m", 4);
	}
	else
		buf_write(1, name, ft_strlen(name));
}
