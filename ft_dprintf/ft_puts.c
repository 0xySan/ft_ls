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

int	ft_putchar(int c)
{
	return (write (1, &c, 1));
}

int	ft_putstring(char *str)
{
	int	i;

	if (!str)
		str = "(null)";
	i = 0;
	while (str[i] != '\0')
	{
		write(1, &str[i], 1);
		i++;
	}
	return (i);
}

int	ft_putnumber(long n, int base)
{
	int		count;
	char	*symbols;

	count = 0;
	symbols = "0123456789abcdef";
	if (n == 0)
		return (ft_putchar('0'));
	if (n == -2147483648)
	{
		write (1, "-2147483648", 11);
		count = 11;
		return (count);
	}
	else if (n < 0)
	{
		ft_putchar ('-');
		n = -n;
		count++;
	}
	if (n >= base)
		count += ft_putnumber(n / base, base);
	count += ft_putchar(symbols[n % base]);
	return (count);
}

int	ft_putnumber_caps(long n, int base)
{
	int		count;
	char	*symbols;

	count = 0;
	symbols = "0123456789ABCDEF";
	if (n == -2147483648)
	{
		write (1, "-2147483648", 12);
		count = 11;
	}
	else if (n < 0)
	{
		ft_putchar ('-');
		n = -n;
	}
	else if (n < base)
		return (ft_putchar (symbols[n]));
	else
	{
		count = ft_putnumber_caps(n / base, base);
		return (count + ft_putnumber_caps (n % base, base));
	}
	return (count);
}
