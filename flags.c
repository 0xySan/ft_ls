/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 19:03:33 by etaquet           #+#    #+#             */
/*   Updated: 2025/06/24 20:12:49 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf/ft_dprintf.h"
#include "ft_ls.h"

void	put_message_help(char *str)
{
	ft_dprintf(1, "Usage: %s [OPTION]... [FILE]...\n", str);
	ft_dprintf(1, "Options:\n");
	ft_dprintf(1, "  -a\t\tdo not ignore entries starting with .\n");
	ft_dprintf(1, "  -d\t\tlist directories themselves, not their contents\n");
	ft_dprintf(1, "  -f\t\tdo not sort, enable -aU, disable -ls --color\n");
	ft_dprintf(1, "  -g\t\tlike -l, but do not list owner\n");
	ft_dprintf(1, "  -G\t\tin a long listing, don't print group names\n");
	ft_dprintf(1, "  -h\t\tprint sizes in human readable format\n");
	ft_dprintf(1, "     --si\tlikewise, but use powers of 1000 not 1024\n");
	ft_dprintf(1, "  -l\t\tuse a long listing format\n");
	ft_dprintf(1, "  -R\t\tlist subdirectories recursively\n");
	ft_dprintf(1, "  -r\t\treverse order while sorting\n");
	ft_dprintf(1, "  -s\t\tprint size of each file in blocks\n");
	ft_dprintf(1, "  -t\t\tsort by time\n");
	ft_dprintf(1, "  -U\t\tdo not sort; list entries in directory order\n");
	ft_dprintf(1, "  --width=COLS\tset output width to COLS.  0 means no limit\n");
	ft_dprintf(1, "  --help\tdisplay this help and exit\n", 1);
	ft_dprintf(1, "  --color\tenable colorized output\n");
	ft_dprintf(1, "Exit status:\n");
	ft_dprintf(1, "  0  if OK,\n");
	ft_dprintf(1, "  1  if minor problems (e.g., cannot access subdirectory),\n");
	ft_dprintf(1, "  2  if serious trouble (e.g., cannot access command-line argument).\n");
	buf_flush(1);
}

void	init_flags(t_flags *flags)
{
	flags->recursive = 0;
	flags->long_format = 0;
	flags->group = 0;
	flags->owner = 0;
	flags->directory = 0;
	flags->reverse = 0;
	flags->time_sort = 0;
	flags->color = 0;
	flags->all = 0;
	flags->size = 0;
	flags->human_readable = 0;
	flags->file_count = 0;
	flags->error_code = 0;
	flags->last_code = 0;
	flags->width = 0;
	flags->size_unit = 1024.0;
	flags->dir_name = NULL;
	flags->dir_flag = false;
}

int	check_flags(char **av, t_flags *flags, int i, int j)
{
	if (av[i][j] == 'l')
		flags->long_format = 1;
	else if (av[i][j] == 'a')
		flags->all = 1;
	else if (av[i][j] == 'R')
		flags->recursive = 1;
	else if (av[i][j] == 'r')
		flags->reverse = 1;
	else if (av[i][j] == 't')
	{
		flags->not_sorted = 0;
		flags->time_sort = 1;
	}
	else if (av[i][j] == 'g')
	{
		flags->long_format = 1;
		flags->owner = 1;
	}
	else if (av[i][j] == 'G')
		flags->group = 1;
	else if (av[i][j] == 'd')
		flags->directory = 1;
	else if (av[i][j] == 'f')
	{
		flags->all = 1;
		flags->not_sorted = 1;
	}
	else if (av[i][j] == 'U')
		flags->not_sorted = 1;
	else if (av[i][j] == 's')
		flags->size = 1;
	else if (av[i][j] == 'h')
	{
		flags->human_readable = 1;
		flags->size_unit = 1024.0;
	}
	else
	{
		ft_dprintf(2, "%s: illegal option -- '%c'\n", av[0], av[i][j]);
		ft_dprintf(2, "Try 'ft_ls --help' for more informations.\n");
		return (1);
	}
	return (0);
}

int	check_flags_loop(char **av, t_flags *flags, int i)
{
	int	j;

	if (strcmp(av[i], "--help") == 0)
	{
		put_message_help(av[0]);
		return (2);
	}
	if (strcmp(av[i], "--color") == 0)
	{
		flags->color = 1;
		return (0);
	}
	if (strcmp(av[i], "--si") == 0)
	{
		flags->size_unit = 1000.0;
		flags->human_readable = 1;
		return (0);
	}
	j = 1;
	while (av[i][j])
	{
		if (check_flags(av, flags, i, j))
			return (1);
		j++;
	}
	return (0);
}

void	exit_help(int code, t_flags *flags, int count)
{
	int	i;

	i = 0;
	if (code == 0)
		return ;
	while (i < count)
	{
		free(flags->files[i]);
		i++;
	}
	free(flags->files);
	free(flags);
	buf_flush(1);
	if (code == 1)
		exit(1);
	else if (code == 2)
		exit(0);
}
