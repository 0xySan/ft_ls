/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recursive_ls.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 19:00:43 by etaquet           #+#    #+#             */
/*   Updated: 2026/03/01 04:06:44 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static void	print_long_entry(t_files *f, int i, t_flags *fl, t_colwidths cw)
{
	getperms(f->stats[i], f->real_paths[i], fl, cw);
	print_color_name(f->files[i], &f->stats[i], fl->color);
	if (S_ISLNK(f->stats[i].st_mode))
		getsymlink(f->stats[i], f->real_paths[i], fl->color);
	buf_write(1, "\n", 1);
}

static int	count_visible(t_files *files, int all)
{
	int	i;
	int	n;

	n = 0;
	i = -1;
	while (files->files[++i])
	{
		if (!files->files[i] || !files->real_paths[i])
			continue ;
		if (files->files[i][0] == '.' && !all)
			continue ;
		n++;
	}
	return (n);
}

static void	print_file_loop(t_files files, t_flags *flags, int *count)
{
	int			i;
	char		**names;
	struct stat	*col_st;
	int			n;
	t_colwidths	cw;

	n = count_visible(&files, flags->all);
	names = malloc(sizeof(char *) * (n + 1));
	col_st = malloc(sizeof(struct stat) * (n + 1));
	if (!names || !col_st)
		return ;
	cw = (t_colwidths){0, 0, 0, 0};
	if (flags->long_format)
		cw = init_colwidths(&files, flags->owner, flags->all, flags->human_readable);
	n = 0;
	i = -1;
	while (files.files[++i])
	{
		if (!files.files[i] || !files.real_paths[i])
			continue ;
		if (files.files[i][0] == '.' && !flags->all)
			continue ;
		if (flags->long_format)
			print_long_entry(&files, i, flags, cw);
		else
		{
			names[n] = files.files[i];
			col_st[n] = files.stats[i];
		}
		n++;
	}
	if (!flags->long_format)
	{
		names[n] = NULL;
		print_columns(names, n, col_st, flags->color, flags->width);
	}
	*count = n;
	free(names);
	free(col_st);
}

static void	free_files(t_files *files)
{
	free(files->stats);
	free(files->real_paths);
	free(files->files);
}

static int	opendir_and_print(const char *base_path, t_flags *flags, t_files *files)
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
	if (flags->long_format || flags->size)
	{
		if (flags->human_readable)
			ft_dprintf(1, "total %s\n", getblocksize_human_readable(files, flags->size_unit));
		else
			ft_dprintf(1, "total %u\n", getblocksize(files));
	}
	closedir(dir);
	return (1);
}

static void	check_recursion(t_files files, t_flags *flags, int i, int printed_count)
{
	if (!files.real_paths[i])
		return ;
	if (S_ISDIR(files.stats[i].st_mode) && !S_ISLNK(files.stats[i].st_mode))
	{
		if (flags->recursive && printed_count >= 1 && !flags->last_code)
			buf_write(1, "\n", 1);
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
		if (ft_strcmp(files.files[i], ".") == 0
			|| ft_strcmp(files.files[i], "..") == 0)
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
