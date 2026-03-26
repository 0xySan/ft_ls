/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 15:04:49 by etaquet           #+#    #+#             */
/*   Updated: 2026/03/26 21:59:55 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	main_loop(int ac, char **av, t_flags *flags)
{
	int	i;
	int exit_code;

	i = 1;
	while (i < ac)
	{
		if (av[i][0] == '-')
		{
			exit_code = check_width_arg(av[i], av[i + 1], flags, &i);
			if (exit_code == 0)
				continue ;
			exit_code = check_hyperlink_arg(av[i], flags, &i);
			if (exit_code == 0)
				continue ;
			exit_code = check_color_arg(av[i], flags, &i);
			if (exit_code == 0)
				continue ;
			exit_help(check_flags_loop(av, flags, i), flags, flags->file_count);
		}
		else
		{
			if (file_exists(av[i]) == false)
			{
				ft_dprintf(2, "ft_ls: cannot access '%s': No such file or directory\n", av[i]);
				flags->error_code = 2;
			}
			flags->files[flags->file_count++] = ft_strdup(av[i]);
		}
		i++;
	}
	if (flags->file_count < 1 && flags->error_code == 0)
		flags->files[flags->file_count++] = ft_strdup(".");
	flags->files[flags->file_count] = NULL;
	if (flags->color == 2)
		flags->color = isatty(1);
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
	buf_flush(1);
	i = -1;
	while (flags->files[++i])
		free(flags->files[i]);
	exit_code = flags->error_code;
	free(flags->files);
	free(flags);
	return (exit_code);
}
