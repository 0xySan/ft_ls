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

char *get_size_human_readable(off_t size, double size_unit)
{
	const char	*units[] = {"", "K", "M", "G", "T", "P", "E", "Z", "Y"};
	int			i;
	static char	buf[32];
	int			len;
	double		fraction;
	int			whole;
	int			decimal;

	i = 0;
	if (size == 0)
	{
		ft_strcpy(buf, "0");
		return (buf);
	}
	fraction = (double)size;
	while (fraction >= size_unit && i < 8)
	{
		fraction /= size_unit;
		i++;
	}
	whole = (int)fraction;
	{
		double tenths = (fraction - whole) * 10.0;
		decimal = (int)tenths;
		if (tenths - decimal > 0.001)
			decimal++;
	}
	if (decimal >= 10)
	{
		whole++;
		decimal = 0;
	}
	if (whole < 10 && i > 0)
	{
		len = ft_strlen(ft_itoa(whole));
		ft_strcpy(buf, ft_itoa(whole));
		buf[len] = '.';
		ft_strcpy(buf + len + 1, ft_itoa(decimal));
		ft_strcpy(buf + len + 2, units[i]);
	}
	else
	{
		len = ft_strlen(ft_itoa(whole));
		ft_strcpy(buf, ft_itoa(whole));
		ft_strcpy(buf + len, units[i]);
	}
	return (buf);
}

char	*getblocksize_human_readable(t_files *files, double size_unit)
{
	size_t		blocksize;
	const char	*units[] = {"K", "M", "G", "T", "P", "E", "Z", "Y"};
	int			i;
	static char	buf[32];
	double		fraction;
	int			whole;
	int			decimal;
	int			len;

	blocksize = getblocksize(files);
	if (blocksize == 0)
	{
		ft_strcpy(buf, "0");
		return (buf);
	}
	i = 0;
	fraction = (double)blocksize;
	while (fraction >= size_unit && i < 8)
	{
		fraction /= size_unit;
		i++;
	}
	whole = (int)fraction;
	{
		double tenths = (fraction - whole) * 10.0;
		decimal = (int)tenths;
		if (tenths - decimal > 0.001)
			decimal++;
	}
	if (decimal >= 10)
	{
		whole++;
		decimal = 0;
	}
	if (whole < 10)
	{
		len = ft_strlen(ft_itoa(whole));
		ft_strcpy(buf, ft_itoa(whole));
		buf[len] = '.';
		ft_strcpy(buf + len + 1, ft_itoa(decimal));
		ft_strcpy(buf + len + 2, units[i]);
	}
	else
	{
		len = ft_strlen(ft_itoa(whole));
		ft_strcpy(buf, ft_itoa(whole));
		ft_strcpy(buf + len, units[i]);
	}
	return (buf);
}

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