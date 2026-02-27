/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puts.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet < etaquet@student.42lehavre.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 22:03:21 by etaquet           #+#    #+#             */
/*   Updated: 2026/02/02 22:17:41 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"

int	ft_putchar(int fd, int c)
{
	return (buf_write(fd, (char *)&c, 1));
}

int	ft_putstring(int fd, char *str)
{
	int	i;

	if (!str)
		str = "(null)";
	i = 0;
	while (str[i] != '\0')
		i++;
	buf_write(fd, str, i);
	return (i);
}

int	ft_putnumber(int fd, long n, int base)
{
	int		count;
	char	*symbols;

	count = 0;
	symbols = "0123456789abcdef";
	if (n == 0)
		return (ft_putchar(fd, '0'));
	if (n == -2147483648)
	{
		buf_write(fd, "-2147483648", 11);
		count = 11;
		return (count);
	}
	else if (n < 0)
	{
		ft_putchar (fd, '-');
		n = -n;
		count++;
	}
	if (n >= base)
		count += ft_putnumber(fd, n / base, base);
	count += ft_putchar(fd, symbols[n % base]);
	return (count);
}

int	ft_putnumber_caps(int fd, long n, int base)
{
	int		count;
	char	*symbols;

	count = 0;
	symbols = "0123456789ABCDEF";
	if (n == -2147483648)
	{
		buf_write(fd, "-2147483648", 11);
		count = 11;
	}
	else if (n < 0)
	{
		ft_putchar (fd, '-');
		n = -n;
	}
	else if (n < base)
		return (ft_putchar (fd, symbols[n]));
	else
	{
		count = ft_putnumber_caps(fd, n / base, base);
		return (count + ft_putnumber_caps (fd, n % base, base));
	}
	return (count);
}
