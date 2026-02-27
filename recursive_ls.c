/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recursive_ls.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 19:00:43 by etaquet           #+#    #+#             */
/*   Updated: 2025/06/24 20:16:38 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	print_file_loop(t_files files, t_flags *flags, int *count)
{
	int			i;
	char		**names;
	int			n;
	t_colwidths	cw;

	n = 0;
	i = -1;
	while (files.files[++i])
	{
		if (!files.files[i] || !files.real_paths[i])
			continue ;
		if (strncmp(files.files[i], ".", 1) == 0 && !flags->all)
			continue ;
		n++;
	}
	names = malloc(sizeof(char *) * (n + 1));
	if (!names)
		return ;
	cw = (t_colwidths){0, 0, 0, 0};
	if (flags->long_format)
		cw = init_colwidths(&files, flags->group, flags->all);
	n = 0;
	i = -1;
	while (files.files[++i])
	{
		if (!files.files[i] || !files.real_paths[i])
			continue ;
		if (strncmp(files.files[i], ".", 1) == 0 && !flags->all)
			continue ;
		if (flags->long_format)
		{
			getperms(files.stats[i], files.real_paths[i], flags->group, cw);
			ft_dprintf(1, "%s", files.files[i]);
			if (S_ISLNK(files.stats[i].st_mode))
				getsymlink(files.stats[i], files.real_paths[i]);
			write(1, "\n", 1);
		}
		else
			names[n] = files.files[i];
		n++;
	}
	if (!flags->long_format)
	{
		names[n] = NULL;
		print_columns(names, n);
	}
	*count = n;
	free(names);
}

static void	free_files(t_files *files)
{
	free(files->stats);
	free(files->real_paths);
	free(files->files);
}

int	opendir_and_print(const char *base_path, t_flags *flags, t_files *files)
{
	DIR		*dir;

	dir = opendir(base_path);
	if (!dir)
	{
		ft_dprintf(2,
			"./ft_ls: cannot open directory '%s': Permission denied\n",
			base_path);
		flags->error_code = 1;
		flags->last_code = 1;
		return (0);
	}
	flags->last_code = 0;
	if (flags->file_count - 1 >= 1 || flags->recursive)
		ft_dprintf(1, "%s:\n", base_path);
	malloc_and_put_files(files, dir, flags, base_path);
	if (flags->long_format)
		ft_dprintf(1, "total %u\n", getblocksize(files));
	closedir(dir);
	return (1);
}

void	check_recursion(t_files files, t_flags *flags, int i, int printed_count)
{
	if (!files.real_paths[i])
		return ;
	if (is_directory(files.real_paths[i]) && !S_ISLNK(files.stats[i].st_mode))
	{
		if (flags->recursive && printed_count >= 1 && !flags->last_code)
			ft_dprintf(1, "\n");
		if (flags->recursive)
			recursive_ls(files.real_paths[i], flags);
	}
}

void	recursive_ls(const char *base_path, t_flags *flags)
{
	t_files	files;
	int		printed_count;
	int		i;

	if (!opendir_and_print(base_path, flags, &files))
		return ;
	printed_count = 0;
	print_file_loop(files, flags, &printed_count);
	i = -1;
	while (files.files[++i])
	{
		if (!files.files[i] || !files.real_paths[i])
			continue ;
		if (strcmp(files.files[i], ".") == 0
			|| strcmp(files.files[i], "..") == 0)
		{
			free(files.files[i]);
			free(files.real_paths[i]);
			continue ;
		}
		check_recursion(files, flags, i, printed_count);
		free(files.files[i]);
		free(files.real_paths[i]);
	}
	free_files(&files);
}
