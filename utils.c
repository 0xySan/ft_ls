/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:34:03 by etaquet           #+#    #+#             */
/*   Updated: 2026/03/13 22:03:49 by etaquet          ###   ########.fr       */
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

static int	utoa_str(unsigned long n, char *dst)
{
	char	tmp[21];
	int		idx;
	int		len;

	idx = 0;
	if (n == 0)
	{
		dst[0] = '0';
		dst[1] = '\0';
		return (1);
	}
	while (n > 0)
	{
		tmp[idx++] = (char)('0' + (n % 10));
		n /= 10;
	}
	len = idx;
	while (idx > 0)
		*dst++ = tmp[--idx];
	*dst = '\0';
	return (len);
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

static char	*humanize_bytes(double bytes, double size_unit)
{
	const char	*units_bin[] = {"", "K", "M", "G", "T", "P", "E", "Z", "Y"};
	const char	*units_si[] = {"", "k", "M", "G", "T", "P", "E", "Z", "Y"};
	const char	**units;
	int			i;
	static char	buf[32];
	int			len;
	double		fraction;
	int			whole;
	int			decimal;
	char		num_buf[12];

	if (bytes == 0)
	{
		ft_strcpy(buf, "0");
		return (buf);
	}
	units = (size_unit == 1000.0) ? units_si : units_bin;
	i = 0;
	fraction = bytes;
	while (fraction >= size_unit && i < 8)
	{
		fraction /= size_unit;
		i++;
	}
	if (i > 0 && fraction < 10.0)
	{
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
			len = utoa_str((unsigned long)whole, num_buf);
			ft_strcpy(buf, num_buf);
			buf[len] = '.';
			utoa_str((unsigned long)decimal, num_buf);
			ft_strcpy(buf + len + 1, num_buf);
			ft_strcpy(buf + len + 2, units[i]);
			return (buf);
		}
	}
	whole = (int)fraction;
	if (i > 0 && fraction - whole > 0.0)
		whole++;
	if (i > 0 && whole >= (int)size_unit && i < 8)
	{
		whole = 1;
		i++;
	}
	if (i > 0)
	{
		len = utoa_str((unsigned long)whole, num_buf);
		ft_strcpy(buf, num_buf);
		ft_strcpy(buf + len, units[i]);
	}
	else
	{
		len = utoa_str((unsigned long)whole, num_buf);
		ft_strcpy(buf, num_buf);
		ft_strcpy(buf + len, units[i]);
	}
	return (buf);
}

char	*get_size_human_readable(off_t size, double size_unit)
{
	return (humanize_bytes((double)size, size_unit));
}

char	*get_blocks_human_readable(size_t blocks, double size_unit)
{
	return (humanize_bytes((double)blocks * 1024.0, size_unit));
}

char	*getblocksize_human_readable(t_files *files, double size_unit)
{
	size_t	blocksize;

	blocksize = getblocksize(files);
	return (humanize_bytes((double)blocksize * 1024.0, size_unit));
}

void	print_block_prefix(struct stat st, int width, int human, double size_unit)
{
	size_t	blocks;

	blocks = (size_t)(st.st_blocks / 2);
	if (human)
	{
		char *hr = get_blocks_human_readable(blocks, size_unit);
		int pad = width - ft_strlen(hr);
		while (pad-- > 0)
			buf_write(1, " ", 1);
		buf_write(1, hr, ft_strlen(hr));
	}
	else
		print_padded_num((unsigned long)blocks, width);
	buf_write(1, " ", 1);
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
