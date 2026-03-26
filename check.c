/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 20:08:08 by etaquet           #+#    #+#             */
/*   Updated: 2026/03/26 21:02:21 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf/ft_dprintf.h"
#include "ft_ls.h"

static void width_empty(t_flags *flags)
{
	ft_dprintf(2, "ft_ls: option '--width' requires an argument\n");
	ft_dprintf(2, "Try 'ft_ls --help' for more informations.\n");
	exit_help(1, flags, flags->file_count);
}

static void width_error(const char *arg, t_flags *flags)
{
	ft_dprintf(2, "ft_ls: invalid line width: ‘%s’\n", arg);
	exit_help(1, flags, flags->file_count);
}

int check_width_arg(const char *arg, const char *argtwo, t_flags *flags, int *i)
{
	if (ft_strncmp(arg, "--width=", 8) == 0 || ft_strncmp(arg, "-w=", 3) == 0)
	{
		const char *num_str = arg + ((arg[1] == 'w') ? 3 : 8);
		if (*num_str == '\0')
			width_error(num_str, flags);
		for (const char *p = num_str; *p; p++)
		{
			if (!isdigit((unsigned char)*p))
				width_error(num_str, flags);
		}
		if (!flags->one)
			flags->width = ft_atoi(num_str);
		(*i)++;
		return 0;
	}
	else if (ft_strcmp(arg, "--width") == 0 || ft_strcmp(arg, "-w") == 0)
	{
		if (argtwo == NULL || argtwo[0] == '\0' || argtwo[0] == '-')
			width_empty(flags);
		for (const char *p = argtwo; *p; p++)
		{
			if (!isdigit((unsigned char)*p))
				width_error(argtwo, flags);
		}
		if (!flags->one)
			flags->width = ft_atoi(argtwo);
		(*i) += 2;
		return 0;
	}
	return -1;
}

int check_hyperlink_arg(const char *arg, t_flags *flags, int *i)
{
	if (ft_strncmp(arg, "--hyperlink=", 12) == 0)
	{
		if (!arg[12])
		{
			ft_dprintf(2, "%s: option requires an argument -- 'hyperlink'\n", arg);
			ft_dprintf(2, "Try 'ft_ls --help' for more informations.\n");
			exit_help(1, flags, flags->file_count);
		}
		char *val = (char *)arg + 12;
		if (strcmp(val, "always") == 0 || strcmp(val, "yes") == 0 || strcmp(val, "force") == 0)
			flags->hyperlink = 1;
		else if (strcmp(val, "auto") == 0 || strcmp(val, "if-tty") == 0 || strcmp(val, "tty") == 0)
			flags->hyperlink = 1;
		else if (strcmp(val, "never") == 0 || strcmp(val, "no") == 0 || strcmp(val, "none") == 0)
			flags->hyperlink = 0;
		else
		{
			ft_dprintf(2, "%s: invalid argument '%s' for '--hyperlink'\n", arg, val);
			ft_dprintf(2, "Valid arguments are:\n");
			ft_dprintf(2, "  - 'always', 'yes', 'force'\n");
			ft_dprintf(2, "  - 'never', 'no', 'none'\n");
			ft_dprintf(2, "  - 'auto', 'tty', 'if-tty'\n");
			ft_dprintf(2, "Try 'ft_ls --help' for more information.\n");
			exit_help(1, flags, flags->file_count);
		}
	}
	else if (ft_strcmp(arg, "--hyperlink") == 0)
		flags->hyperlink = 1;
	else
		return -1;
	(*i)++;
	return 0;
}