/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet < etaquet@student.42lehavre.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 15:31:45 by ashobajo          #+#    #+#             */
/*   Updated: 2026/02/02 22:17:18 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>

int	ft_dprintf(int fd, const char *format, ...);
int	ft_putchar(int fd, int c);
int	ft_putnumber(int fd, long n, int base);
int	ft_putnumber_caps(int fd, long n, int base);
int	print_format(int fd, char specifier, va_list ap);
int	ft_putunit_max(int fd, unsigned long num, int base);
int	ft_putpointer(int fd, void *pointer);
int	ft_putstring(int fd, char *str);

#endif
