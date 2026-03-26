/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_flags.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 03:50:40 by etaquet           #+#    #+#             */
/*   Updated: 2026/03/26 22:57:53 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_ls.h"

void	do_other_flag(t_flags *flags, const char *filename)
{
	struct stat		st;
	t_colwidths		cw;
	char			*hyperlink;

	if (lstat(filename, &st) != 0)
		return ;
	if (flags->long_format)
	{
		cw = (t_colwidths){0, 0, 0, 0, 0};
		if (flags->human_readable)
			cw.size_w = ft_strlen(get_size_human_readable(st.st_size, flags->size_unit));
		else
			cw.size_w = count_digits((unsigned long)st.st_size);
		if (flags->size)
		{
			size_t blocks = (size_t)(st.st_blocks / 2);
			if (flags->human_readable)
				cw.blocks_w = ft_strlen(get_blocks_human_readable(blocks, flags->size_unit));
			else
				cw.blocks_w = count_digits((unsigned long)blocks);
			print_block_prefix(st, cw.blocks_w, flags->human_readable, flags->size_unit);
		}
		getperms(st, filename, flags, cw);
		hyperlink = NULL;
		if (flags->hyperlink)
			hyperlink = build_hyperlink_path(filename);
		if (flags->hyperlink && hyperlink)
		{
			write_hyperlink_open(hyperlink);
			print_normal_name(filename, filename, &st, flags);
			write_hyperlink_close();
		}
		else
			print_normal_name(filename, filename, &st, flags);
		print_symlink_with_hyperlink(st, filename, flags->color, flags, hyperlink);
		free(hyperlink);
		buf_write(1, "\n", 1);
	}
	else
	{
		print_name_hyperlink(filename, &st, flags, filename);
		if (!isatty(1))
			buf_write(1, "\n", 1);
		else
			buf_write(1, "  ", 2);
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
		do_other_flag(flags, flags->files[i]);
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

void	do_files(t_flags *flags)
{
	int			i;
	char		**non_dirs;
	struct stat	*nd_st;
	int			non_dir_count;
	int			first;

	i = -1;
	non_dirs = malloc(sizeof(char *) * (flags->file_count + 1));
	nd_st = malloc(sizeof(struct stat) * (flags->file_count + 1));
	if (!non_dirs || !nd_st)
		return ;
	non_dir_count = 0;
	first = 1;
	i = -1;
	while (flags->files[++i])
	{
		if (!is_directory(flags->files[i]) || is_symlink(flags->files[i]))
		{
			if (lstat(flags->files[i], &nd_st[non_dir_count]) != 0)
				continue ;
			non_dirs[non_dir_count++] = flags->files[i];
			continue ;
		}
	}
	if (non_dir_count > 0)
	{
		non_dirs[non_dir_count] = NULL;
		if (!flags->not_sorted)
		{
			if (flags->time_sort)
				timesort_files(non_dirs, 0, non_dir_count - 1);
			else
				quicksort_files(non_dirs, 0, non_dir_count - 1);
			if (flags->reverse)
				reverse_files(non_dirs, non_dir_count);
			i = -1;
			while (++i < non_dir_count)
				lstat(non_dirs[i], &nd_st[i]);
		}
		if (flags->long_format)
		{
			i = -1;
			while (non_dirs[++i])
				do_other_flag(flags, non_dirs[i]);
		}
		else
			print_columns(non_dirs, non_dirs, non_dir_count, nd_st, flags);
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
	free(nd_st);
}

void	do_flags(t_flags *flags)
{
	int			i;
	char		**non_dirs;
	struct stat	*nd_st;
	int			non_dir_count;
	int			first;

	i = -1;
	if (flags->directory)
	{
		i = -1;
		while (flags->files[++i])
		{
			do_other_flag(flags, flags->files[i]);
			if (!flags->long_format && isatty(1))
				continue ;
			if (!isatty(1) || flags->long_format)
				continue ;
		}
		if (!flags->long_format && isatty(1))
			buf_write(1, "\n", 1);
		return ;
	}
	if (flags->recursive && flags->file_count >= 1)
		do_recursive_flag(flags);
	else
		do_files(flags);
}
