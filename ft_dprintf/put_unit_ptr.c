/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_unit_ptr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet < etaquet@student.42lehavre.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 22:03:08 by etaquet           #+#    #+#             */
/*   Updated: 2026/02/02 22:17:35 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"

int	ft_putunit_max(int fd, unsigned long num, int base)
{
	char	*digits;
	char	buffer[50];
	int		i;
	int		count;

	digits = "0123456789abcdef";
	i = 49;
	count = 0;
	buffer[i] = '\0';
	if (num == 0)
		buffer[--i] = '0';
	while (num > 0)
	{
		buffer[--i] = digits[num % base];
		num /= base;
	}
	count = 49 - i;
	buf_write(fd, buffer + i, count);
	return (count);
}

int	ft_putpointer(int fd, void *pointer)
{
	unsigned long long	address;
	int					count;

	count = 0;
	address = (unsigned long long) pointer;
	if (pointer == NULL)
		return (ft_putstring(fd, "(nil)"));
	count += ft_putstring(fd, "0x");
	count += ft_putunit_max(fd, address, 16);
	return (count);
}
