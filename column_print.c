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

#include "ft_ls.h"

static int	get_term_width(void)
{
	struct winsize	ws;

	if (ioctl(1, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
		return (80);
	return (ws.ws_col);
}

static void	pad_write(const char *str, int width)
{
	int	len;
	int	i;

	len = ft_strlen(str);
	ft_dprintf(1, "%s", str);
	i = len;
	while (i < width)
	{
		write(1, " ", 1);
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
			if (idx < count && ft_strlen(names[idx]) > cw[c])
				cw[c] = ft_strlen(names[idx]);
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

void	print_columns(char **names, int count)
{
	int	term_w;
	int	nrows;
	int	ncols;
	int	*col_w;
	int	r;
	int	c;
	int	idx;

	if (count <= 0)
		return ;
	if (!isatty(1))
	{
		r = -1;
		while (++r < count)
			if (names[r])
				ft_dprintf(1, "%s\n", names[r]);
		return ;
	}
	term_w = get_term_width();
	col_w = malloc(sizeof(int) * (count + 1));
	if (!col_w)
		return ;
	nrows = 0;
	while (++nrows <= count)
		if (try_layout(names, count, nrows, col_w, term_w))
			break ;
	ncols = (count + nrows - 1) / nrows;
	r = -1;
	while (++r < nrows)
	{
		c = -1;
		while (++c < ncols)
		{
			idx = c * nrows + r;
			if (idx >= count || !names[idx])
				break ;
			if (c == ncols - 1 || (c + 1) * nrows + r >= count)
				ft_dprintf(1, "%s", names[idx]);
			else
				pad_write(names[idx], col_w[c] + 2);
		}
		write(1, "\n", 1);
	}
	free(col_w);
}
