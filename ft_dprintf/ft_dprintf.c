/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet < etaquet@student.42lehavre.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 22:03:27 by etaquet           #+#    #+#             */
/*   Updated: 2026/02/02 22:17:43 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"

int	ft_dprintf(int fd, const char *format, ...)
{
	va_list	ap;
	int		count;

	count = 0;
	va_start (ap, format);
	while (*(format) != '\0')
	{
		if (*format == '%' && *(format + 1) == '%')
			count += buf_write(fd, format++, 1);
		else if (*format == '%')
			count += print_format (fd, *(++format), ap);
		else
			count += buf_write(fd, format, 1);
		format++;
	}
	va_end (ap);
	return (count);
}
