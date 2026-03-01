/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   column_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 00:00:00 by etaquet           #+#    #+#             */
/*   Updated: 2026/02/27 00:00:00 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_ls.h"

static int	get_term_width(void)
{
	struct winsize	ws;

	if (ioctl(1, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
		return (80);
	return (ws.ws_col);
}

static void	pad_write(const char *s, int w, const char *cc)
{
	int	len;
	int	i;

	len = ft_strlen(s);
	if (cc)
		ft_dprintf(1, "%s%s\033[0m", cc, s);
	else
		ft_dprintf(1, "%s", s);
	i = len;
	while (i < w)
	{
		buf_write(1, " ", 1);
		i++;
	}
}

static int	try_layout(char **names, int count, int nrows, int *cw, int tw)
{
	int	ncols;
	int	c;
	int	r;
	int	idx;
	int	total;

	ncols = (count + nrows - 1) / nrows;
	c = -1;
	while (++c < ncols)
	{
		cw[c] = 0;
		r = -1;
		while (++r < nrows)
		{
			idx = c * nrows + r;
			if (idx < count && (int)ft_strlen(names[idx]) > cw[c])
				cw[c] = (int)ft_strlen(names[idx]);
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

static void	print_col_pipe(char **nm, int cnt, struct stat *st, int col)
{
	int	i;

	i = -1;
	while (++i < cnt)
	{
		if (!nm[i])
			continue ;
		if (col && st)
			print_color_name(nm[i], &st[i], col);
		else
			ft_dprintf(1, "%s", nm[i]);
		buf_write(1, "\n", 1);
	}
}

static void	print_col_row(char **nm, struct stat *st, int *p, const char *cc)
{
	int	idx;

	idx = p[0];
	if (st && p[3])
		cc = get_color_code(&st[idx]);
	if (p[2])
	{
		if (cc)
			ft_dprintf(1, "%s%s\033[0m", cc, nm[idx]);
		else
			ft_dprintf(1, "%s", nm[idx]);
	}
	else
		pad_write(nm[idx], p[1] + 2, cc);
}

void	print_columns(char **nm, int cnt, struct stat *st, int col, int width)
{
	int	v[5];
	int	*cw;

	if (cnt <= 0)
		return ;
	if (!isatty(1))
		return (print_col_pipe(nm, cnt, st, col));
	if (width > 0)
		v[0] = width;
	else
		v[0] = get_term_width();
	cw = malloc(sizeof(int) * (cnt + 1));
	if (!cw)
		return ;
	v[1] = 0;
	while (++v[1] <= cnt)
		if (try_layout(nm, cnt, v[1], cw, v[0]))
			break ;
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
			print_col_row(nm, st, (int [4]){v[0], cw[v[4]], (v[4] == v[2] - 1
				|| (v[4] + 1) * v[1] + v[3] >= cnt), col}, NULL);
		}
		buf_write(1, "\n", 1);
	}
	free(cw);
}
