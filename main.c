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

#include "ft_ls.h"

void	do_other_flag(t_flags *flags, int i)
{
	struct stat	st;

	if (lstat(flags->files[i], &st) != 0)
		return ;
	if (flags->long_format)
	{
		getperms(st);
		ft_dprintf(1, "%s", flags->files[i]);
		if (S_ISLNK(st.st_mode))
			getsymlink(st, flags->files[i]);
	}
	else
		ft_dprintf(1, "%s  ", flags->files[i]);
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
	int	i;
	int	count;

	i = -1;
	count = 0;
	if (flags->recursive && flags->file_count >= 1)
		do_recursive_flag(flags);
	else
	{
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
				ft_putstr_fd(av[0], 2);
				ft_putstr_fd(": cannot access '", 2);
				ft_putstr_fd(av[i], 2);
				ft_putstr_fd("': No such file or directory\n", 2);
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
