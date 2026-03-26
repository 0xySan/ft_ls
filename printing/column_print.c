/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   column_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 00:00:00 by etaquet           #+#    #+#             */
/*   Updated: 2026/03/26 22:45:25 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_ls.h"

static int	name_suffix_len(struct stat *st, t_flags *flags)
{
	if ((!flags->file_type && !flags->classify) || !st)
		return (0);
	if (S_ISDIR(st->st_mode))
		return (1);
	if (S_ISLNK(st->st_mode))
		return (!flags->long_format);
	if (S_ISFIFO(st->st_mode))
		return (1);
	if (S_ISSOCK(st->st_mode))
		return (1);
	#ifdef S_ISDOOR
	if (S_ISDOOR(st->st_mode)) // Not widely supported, so check if it's defined first (Solaris/illumos-specific)
		return (1);
	#endif
	if (st->st_mode & (S_IXUSR | S_IXGRP | S_IXOTH) && flags->file_type)
		return (1);
	return (0);
}

static int	display_name_len(const char *name, struct stat *st, t_flags *flags)
{
	int	len;

	len = ft_strlen(name);
	len += name_suffix_len(st, flags);
	return (len);
}

static int	get_term_width(void)
{
	struct winsize	ws;

	if (ioctl(1, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
		return (80);
	return (ws.ws_col);
}

static int	try_layout(char **names, int count, int nrows, int *cw, int tw,
	int size, int block_w, struct stat *st, t_flags *flags)
{
	int	ncols;
	int	c;
	int	r;
	int	idx;
	int	total;
	int	extra;

	ncols = (count + nrows - 1) / nrows;
	extra = (size ? (block_w + 1) : 0);
	c = -1;
	while (++c < ncols)
	{
		cw[c] = 0;
		r = -1;
		while (++r < nrows)
		{
			idx = c * nrows + r;
			if (idx < count
				&& display_name_len(names[idx], st ? &st[idx] : NULL, flags)
				+ extra > cw[c])
				cw[c] = display_name_len(names[idx], st ? &st[idx] : NULL, flags)
					+ extra;
		}
	}
	total = 0;
	c = -1;
	while (++c < ncols)
	{
		if (c < ncols - 1)
			total += cw[c] + 2;
		else
			total += cw[c];
	}
	return (total <= tw);
}

static void	print_col_pipe(char **nm, char **rp, int cnt, struct stat *st,
	t_flags *flags, int block_w)
{
	int	i;

	i = -1;
	while (++i < cnt)
	{
		if (!nm[i])
			continue ;
		if (flags->size && st)
			print_block_prefix(st[i], block_w, flags->human_readable, flags->size_unit);
		if (st)
			print_name_with_hyperlink(nm[i], rp ? rp[i] : nm[i], &st[i], flags);
		else
			print_name_with_hyperlink(nm[i], rp ? rp[i] : nm[i], NULL, flags);
		buf_write(1, "\n", 1);
	}
}

static void	print_col_row(char **nm, char **rp, struct stat *st, int *p,
	const char *cc, int size, int block_w, t_flags *flags)
{
	int	idx;
	int	name_len;
	int	prefix_len;
	int	pad;

	idx = p[0];
	name_len = display_name_len(nm[idx], st ? &st[idx] : NULL, flags);
	prefix_len = 0;
	if (size && st)
	{
		prefix_len = block_w + 1;
		print_block_prefix(st[idx], block_w, flags->human_readable, flags->size_unit);
	}
	if (st)
		print_name_with_hyperlink(nm[idx], rp ? rp[idx] : nm[idx], &st[idx], flags);
	else
		print_name_with_hyperlink(nm[idx], rp ? rp[idx] : nm[idx], NULL, flags);
	if (p[2])
		return ;
	pad = (p[1] - (prefix_len + name_len)) + 2;
	while (pad-- > 0)
		buf_write(1, " ", 1);
}

static int	get_blocks_width(struct stat *st, int cnt, int human, double size_unit)
{
	int		i;
	int		max;
	int		w;
	size_t	blocks;

	if (!st || cnt <= 0)
		return (0);
	max = 0;
	i = -1;
	while (++i < cnt)
	{
		blocks = (size_t)(st[i].st_blocks / 2);
		if (human)
			w = ft_strlen(get_blocks_human_readable(blocks, size_unit));
		else
			w = count_digits((unsigned long)blocks);
		if (w > max)
			max = w;
	}
	return (max);
}

void	print_columns(char **nm, char **rp, int cnt, struct stat *st, t_flags *flags)
{
	int	v[5];
	int	*cw;
	int	block_w;
	int	found;

	if (cnt <= 0)
		return ;
	block_w = 0;
	if (flags->size)
		block_w = get_blocks_width(st, cnt, flags->human_readable, flags->size_unit);
	if (!isatty(1))
		return (print_col_pipe(nm, rp, cnt, st, flags, block_w));
	if (flags->width > 0)
		v[0] = flags->width;
	else if (flags->width == 0)
		v[0] = 2147483647;
	else
		v[0] = get_term_width();
	cw = malloc(sizeof(int) * (cnt + 1));
	if (!cw)
		return ;
	found = 0;
	v[1] = 0;
	while (++v[1] <= cnt)
	{
		if (try_layout(nm, cnt, v[1], cw, v[0], flags->size, block_w, st, flags))
		{
			found = 1;
			break ;
		}
	}
	if (!found)
	{
		try_layout(nm, cnt, cnt, cw, 2147483647, flags->size, block_w, st, flags);
		v[1] = cnt;
	}
	v[2] = (cnt + v[1] - 1) / v[1];
	v[3] = -1;
	while (++v[3] < v[1])
	{
		v[4] = -1;
		while (++v[4] < v[2])
		{
			v[0] = v[4] * v[1] + v[3];
			if (v[0] >= cnt || !nm[v[0]])
				break ;
			print_col_row(nm, rp, st, (int [4]){v[0], cw[v[4]], (v[4] == v[2] - 1
				|| (v[4] + 1) * v[1] + v[3] >= cnt), flags->color}, NULL,
				flags->size, block_w, flags);
		}
		buf_write(1, "\n", 1);
	}
	free(cw);
}
