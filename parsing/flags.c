/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 19:03:33 by etaquet           #+#    #+#             */
/*   Updated: 2026/04/07 15:48:11 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_ls.h"

void	put_message_help()
{
	ft_dprintf(1, "Usage: ft_ls [OPTION]... [FILE]...\n");
	ft_dprintf(1, "List information about the FILEs (the current directory by default).\n");
	ft_dprintf(1, "Sort entries alphabetically if none of -ftU is specified.\n\n");
	ft_dprintf(1, "Mandatory arguments to long options are mandatory for short options too.\n");
	ft_dprintf(1, "  -a, --all\t\t\tdo not ignore entries starting with .\n");
	ft_dprintf(1, "  -A, --almost-all\t\tdo not list implied . and ..\n");
	ft_dprintf(1, "      --author\t\t\twith -l, print the author of each file\n");
	ft_dprintf(1, "      --color[=WHEN]\t\tcolorize the output; WHEN can be 'always' (default\n");
	ft_dprintf(1, "\t\t\t\t  if omitted), 'auto', or 'never'; more info below\n");
	ft_dprintf(1, "  -d, --directory\t\tlist directories themselves, not their contents\n");
	ft_dprintf(1, "  -f\t\t\t\tdo not sort, enable -aU, disable -ls --color\n");
	ft_dprintf(1, "  -F, --classify\t\tappend indicator (one of */=>@|) to entries\n");
	ft_dprintf(1, "      --file-type\t\tlikewise, except do not append '*'\n");
	ft_dprintf(1, "  -g\t\t\t\tlike -l, but do not list owner\n");
	ft_dprintf(1, "  -G, --no-group\t\tin a long listing, don't print group names\n");
	ft_dprintf(1, "  -h, --human-readable\t\tprint sizes in human readable format\n");
	ft_dprintf(1, "      --si\t\t\tlikewise, but use powers of 1000 not 1024\n");
	ft_dprintf(1, "      --hyperlink[=WHEN]\thyperlink file names; WHEN can be 'always'\n");
	ft_dprintf(1, "\t\t\t\t  (default if omitted), 'auto', or 'never'\n");
	ft_dprintf(1, "  -l\t\t\t\tuse a long listing format\n");
	ft_dprintf(1, "  -R, --recursive\t\tlist subdirectories recursively\n");
	ft_dprintf(1, "  -r, --reverse\t\t\treverse order while sorting\n");
	ft_dprintf(1, "  -s, --size\t\t\tprint size of each file in blocks\n");
	ft_dprintf(1, "  -t, --time\t\t\tsort by time\n");
	ft_dprintf(1, "  -U\t\t\t\tdo not sort; list entries in directory order\n");
	ft_dprintf(1, "  -w, --width=COLS\t\tset output width to COLS.  0 means no limit\n");
	ft_dprintf(1, "  -1\t\t\t\tlist one file per line.  Avoid '\\n' with -q or -b\n");
	ft_dprintf(1, "      --help\tdisplay this help and exit\n");
	ft_dprintf(1, "      --version\toutput version information and exit\n\n");
	ft_dprintf(1, "Using color to distinguish file types is disabled both by default and\n\
with --color=never.  With --color=auto, ls emits color codes only when\n\
standard output is connected to a terminal.  The LS_COLORS environment\n\
variable can change the settings.  Use the dircolors command to set it.\n\n");
	ft_dprintf(1, "Exit status:\n");
	ft_dprintf(1, "  0  if OK,\n");
	ft_dprintf(1, "  1  if minor problems (e.g., cannot access subdirectory),\n");
	ft_dprintf(1, "  2  if serious trouble (e.g., cannot access command-line argument).\n\n");
	ft_dprintf(1, "Report bugs to: etaquet@student.42lehavre.fr\n");
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
	flags->almost_all = 0;
	flags->not_sorted = 0;
	flags->size = 0;
	flags->human_readable = 0;
	flags->one = 0;
	flags->file_count = 0;
	flags->error_code = 0;
	flags->last_code = 0;
	flags->width = -1;
	flags->size_unit = 1024.0;
	flags->hyperlink = 0;
	flags->file_type = 0;
	flags->classify = 0;
	flags->author = 0;
	flags->dir_name = NULL;
	flags->dir_flag = false;
}

int	check_flags(char **av, t_flags *flags, int i, int j)
{
	if (av[i][j] == 'l')
		flags->long_format = 1;
	else if (av[i][j] == 'a')
	{
		flags->all = 1;
		flags->almost_all = 0;
	}
	else if (av[i][j] == 'A')
	{
		flags->all = 0;
		flags->almost_all = 1;
	}
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
		flags->color = 0;
	}
	else if (av[i][j] == 'F')
	{
		flags->file_type = 1;
		flags->classify = 1;
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
	else if (av[i][j] == '1')
	{
		flags->one = 1;
		flags->width = 1;
	}
	else
	{
		ft_dprintf(2, "ft_ls: illegal option -- '%c'\n", av[i][j]);
		ft_dprintf(2, "Try 'ft_ls --help' for more informations.\n");
		return (1);
	}
	return (0);
}

int check_long_format_flags(char **av, t_flags *flags, int i)
{
	if (ft_strcmp(av[i], "--help") == 0)
	{
		put_message_help();
		return (2);
	}
	else if (ft_strcmp(av[i], "--version") == 0)
	{
		ft_dprintf(1, "ft_ls: version 1.0\n\tMade with love by etaquet\n");
		return (2);
	}
	else if (ft_strcmp(av[i], "--si") == 0)
	{
		flags->size_unit = 1000.0;
		flags->human_readable = 1;
	}
	else if (ft_strcmp(av[i], "--all") == 0)
	{
		flags->all = 1;
		flags->almost_all = 0;
	}
	else if (ft_strcmp(av[i], "--almost-all") == 0)
	{
		flags->all = 0;
		flags->almost_all = 1;
	}
	else if (ft_strcmp(av[i], "--directory") == 0)
		flags->directory = 1;
	else if (ft_strcmp(av[i], "--recursive") == 0)
		flags->recursive = 1;
	else if (ft_strcmp(av[i], "--reverse") == 0)
		flags->reverse = 1;
	else if (ft_strcmp(av[i], "--time") == 0)
		flags->time_sort = 1;
	else if (ft_strcmp(av[i], "--size") == 0)
		flags->size = 1;
	else if (ft_strcmp(av[i], "--author") == 0)
		flags->author = 1;
	else if (ft_strcmp(av[i], "--human-readable") == 0)
	{
		flags->human_readable = 1;
		flags->size_unit = 1024.0;
	}
	else if (ft_strcmp(av[i], "--no-group") == 0)
		flags->group = 1;
	else if (ft_strcmp(av[i], "--file-type") == 0)
	{
		flags->file_type = 0;
		flags->classify = 1;
	}
	else if (ft_strcmp(av[i], "--classify") == 0)
	{
		flags->file_type = 1;
		flags->classify = 1;
	}
	else
	{
		ft_dprintf(2, "ft_ls: unrecognized option '%s'\n", av[i]);
		ft_dprintf(2, "Try 'ft_ls --help' for more informations.\n");
		return (1);
	}
	return (0);
}

int	check_flags_loop(char **av, t_flags *flags, int i)
{
	int	j;

	j = 1;
	while (av[i][j])
	{
		if (strncmp(av[i], "--", 2) == 0)
			return check_long_format_flags(av, flags, i);
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
