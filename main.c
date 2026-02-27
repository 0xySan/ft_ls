/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 15:04:49 by etaquet           #+#    #+#             */
/*   Updated: 2025/07/15 18:59:54 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf/ft_dprintf.h"
#include "ft_ls.h"

void	do_other_flag(t_flags *flags, int i)
{
	struct stat		st;
	t_colwidths		cw;

	if (lstat(flags->files[i], &st) != 0)
		return ;
	if (flags->long_format)
	{
		cw = (t_colwidths){0, 0, 0, 0};
		getperms(st, flags->files[i], flags->group, cw);
		ft_dprintf(1, "%s", flags->files[i]);
		if (S_ISLNK(st.st_mode))
			getsymlink(st, flags->files[i]);
		write(1, "\n", 1);
	}
	else
	{
		ft_dprintf(1, "%s", flags->files[i]);
		if (!isatty(1))
			write(1, "\n", 1);
		else
			write(1, "  ", 2);
	}
}

void	do_recursive_flag(t_flags *flags)
{
	int	i;
	int	count;

	count = 0;
	i = -1;
	while (flags->files[++i])
	{
		if (is_directory(flags->files[i]) && !is_symlink(flags->files[i]))
			continue ;
		do_other_flag(flags, i);
		count++;
	}
	if (count >= 1)
		ft_dprintf(1, "\n");
	i = -1;
	while (flags->files[++i])
	{
		if (!is_directory(flags->files[i]) || is_symlink(flags->files[i]))
			continue ;
		if (count >= 1)
			ft_dprintf(1, "\n");
		recursive_ls(flags->files[i], flags);
		count++;
	}
}

void	do_flags(t_flags *flags)
{
	int		i;
	char	**non_dirs;
	int		non_dir_count;
	int		first;

	i = -1;
	if (flags->directory)
	{
		i = -1;
		while (flags->files[++i])
		{
			do_other_flag(flags, i);
			if (!flags->long_format && isatty(1))
				continue ;
			if (!isatty(1) || flags->long_format)
				continue ;
		}
		if (!flags->long_format && isatty(1))
			write(1, "\n", 1);
		return ;
	}
	if (flags->recursive && flags->file_count >= 1)
		do_recursive_flag(flags);
	else
	{
		non_dirs = malloc(sizeof(char *) * (flags->file_count + 1));
		if (!non_dirs)
			return ;
		non_dir_count = 0;
		first = 1;
		i = -1;
		while (flags->files[++i])
		{
			if (!is_directory(flags->files[i]) || is_symlink(flags->files[i]))
			{
				non_dirs[non_dir_count++] = flags->files[i];
				continue ;
			}
		}
		if (non_dir_count > 0)
		{
			non_dirs[non_dir_count] = NULL;
			print_columns(non_dirs, non_dir_count);
			first = 0;
		}
		i = -1;
		while (flags->files[++i])
		{
			if (!is_directory(flags->files[i]) || is_symlink(flags->files[i]))
				continue ;
			if (!first)
				ft_dprintf(1, "\n");
			recursive_ls(flags->files[i], flags);
			first = 0;
		}
		free(non_dirs);
	}
}

void	main_loop(int ac, char **av, t_flags *flags)
{
	int	i;

	i = 1;
	while (i < ac)
	{
		if (av[i][0] == '-')
			exit_help(check_flags_loop(av, flags, i), flags, flags->file_count);
		else
		{
			if (file_exists(av[i]) == false)
			{
				ft_dprintf(2, "%s: cannot access '%s': No such file or directory\n", av[0], av[i]);
				flags->error_code = 2;
			}
			flags->files[flags->file_count++] = ft_strdup(av[i]);
		}
		i++;
	}
	if (flags->file_count < 1 && flags->error_code == 0)
		flags->files[flags->file_count++] = ft_strdup(".");
	flags->files[flags->file_count] = NULL;
}

int	main(int ac, char **av)
{
	int		i;
	int		exit_code;
	t_flags	*flags;

	flags = malloc(sizeof(t_flags));
	if (!flags)
		return (1);
	flags->files = malloc(sizeof(char *) * (count_files(av) + 1));
	if (!flags->files)
		return (1);
	init_flags(flags);
	flags->file_count = 0;
	main_loop(ac, av, flags);
	if (flags->time_sort && flags->file_count > 1)
		timesort_files(flags->files, 0, flags->file_count - 1);
	else
		quicksort_files(flags->files, 0, flags->file_count - 1);
	do_flags(flags);
	i = -1;
	while (flags->files[++i])
		free(flags->files[i]);
	exit_code = flags->error_code;
	free(flags->files);
	free(flags);
	return (exit_code);
}
