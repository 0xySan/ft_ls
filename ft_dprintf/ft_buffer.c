/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_buffer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 00:00:00 by etaquet           #+#    #+#             */
/*   Updated: 2026/02/27 00:00:00 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"

static char	g_buf[BUF_SIZE];
static int	g_pos = 0;
static int	g_fd = 1;

void	buf_flush(int fd)
{
	(void)fd;
	if (g_pos > 0)
	{
		write(g_fd, g_buf, g_pos);
		g_pos = 0;
	}
}

static void	ft_memcpy(char *dst, const char *src, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		dst[i] = src[i];
		i++;
	}
}

int	buf_write(int fd, const char *s, int len)
{
	int	space;
	int	off;

	if (fd != g_fd)
	{
		buf_flush(g_fd);
		g_fd = fd;
	}
	off = 0;
	while (off < len)
	{
		space = BUF_SIZE - g_pos;
		if (space <= 0)
		{
			buf_flush(fd);
			space = BUF_SIZE;
		}
		if (len - off < space)
			space = len - off;
		ft_memcpy(g_buf + g_pos, s + off, space);
		g_pos += space;
		off += space;
	}
	return (len);
}
