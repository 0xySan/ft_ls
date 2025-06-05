/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 15:04:49 by etaquet           #+#    #+#             */
/*   Updated: 2025/06/05 18:01:24 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

bool	file_exists(char *filename)
{
	struct stat	buffer;

	return (stat(filename, &buffer) == 0);
}

int	ft_strlen(const char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

char *ft_strdup(const char *s1)
{
	char	*copy;
	int		i;

	if (!s1)
		return (NULL);
	copy = malloc(sizeof(char) * (ft_strlen(s1) + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		copy[i] = s1[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}

void	put_message_help(char *str)
{
	ft_putstr_fd("Usage: ", 1);
	ft_putstr_fd(str, 1);
	ft_putstr_fd(" [OPTION]... [FILE]...\n", 1);
	ft_putstr_fd("Options:\n", 1);
	ft_putstr_fd("  -a\t\tdo not ignore entries starting with .\n", 1);
	ft_putstr_fd("  -l\t\tuse a long listing format\n", 1);
	ft_putstr_fd("  -R\t\tlist subdirectories recursively\n", 1);
	ft_putstr_fd("  -r\t\treverse order while sorting\n", 1);
	ft_putstr_fd("  -t\t\tsort by time\n", 1);
	ft_putstr_fd("  --help\tshow this help message\n", 1);
}

int count_files(char **av)
{
	int count;
	int i;

	i = 0;
	count = 0;
	while (av[i])
	{
		if (av[i][0] && av[i][0] != '-' && file_exists(av[i]))
			count++;
		i++;
	}
	return count;
}

void init_flags(t_flags *flags)
{
	flags->recursive = 0;
	flags->long_format = 0;
	flags->reverse = 0;
	flags->time_sort = 0;
	flags->all = 0;
	flags->file_count = 0;
	flags->colors = 0;
	flags->dir_name = NULL;
	flags->dir_flag = false;
}

int check_flags(char **av, t_flags *flags, int i, int j)
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
		flags->time_sort = 1;
	else if (av[i][j] == 'c')
		flags->colors = 1;
	else
	{
		ft_putstr_fd(av[0], 2);
		ft_putstr_fd(": illegal option -- '", 2);
		write(1, &av[i][j], 2);
		ft_putstr_fd("'\nTry 'ft_ls --help' for more informations.\n", 2);
		free(flags->files);
		free(flags);
		return (1);
	}
	return (0);
}

int check_flags_loop(char **av, t_flags *flags, int i)
{
	int j;

	if (strcmp(av[i], "--help") == 0)
	{
		put_message_help(av[0]);
		return (2);
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

void exit_help(int code, t_flags *flags, int count)
{
	int i;

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
	if (code == 1)
		exit(1);
	else if (code == 2)
		exit(0);
}

void	malloc_and_put_files(t_files *files, DIR *dir, t_flags *flags, const char *bp)
{
	int				count;
	struct dirent	*entry;

	count = 0;
	while ((entry = readdir(dir)) != NULL)
	{
		if (strncmp(entry->d_name, ".", 1) == 0 && !flags->all)
			continue ;
		count++;
	}
	files->files = malloc(sizeof(char *) * (count + 1));
	if (!files->files)
		exit(1);
	files->real_paths = malloc(sizeof(char *) * (count + 1));
	if (!files->real_paths)
		exit(1);
	files->stats = calloc(count, sizeof(struct stat));
	if (!files->stats)
		exit(1);
	rewinddir(dir);
	count = 0;
	while ((entry = readdir(dir)) != NULL)
	{
		if (strncmp(entry->d_name, ".", 1) == 0 && !flags->all)
			continue ;
		files->files[count] = strdup(entry->d_name);
		files->real_paths[count] = get_real_path(bp, entry->d_name);
		if (lstat(files->real_paths[count], &files->stats[count]) == -1)
			perror(files->real_paths[count]);
		count++;
	}
	files->files[count] = NULL;
	files->real_paths[count] = NULL;
	if (!flags->time_sort)
		quicksort(files->files, files->real_paths, files->stats, 0, count - 1);
	else
		timesort(files->files, files->real_paths, files->stats,  0, count - 1);
	if (flags->reverse)
	{
		for (int i = 0; i < count / 2; i++)
		{
			char *tmp = files->files[i];
			files->files[i] = files->files[count - 1 - i];
			files->files[count - 1 - i] = tmp;

			tmp = files->real_paths[i];
			files->real_paths[i] = files->real_paths[count - 1 - i];
			files->real_paths[count - 1 - i] = tmp;

			struct stat tmp_stat = files->stats[i];
			files->stats[i] = files->stats[count - 1 - i];
			files->stats[count - 1 - i] = tmp_stat;
		}
	}
}

void recursive_ls(const char *base_path, t_flags *flags)
{
	DIR				*dir;
	int				count;
	t_files			files;
	int				i;

	count = 0;
	i = -1;
	dir = opendir(base_path);
	if (!dir)
		return ;
	if (flags->file_count - 1 >= 1 || flags->recursive)
		ft_dprintf(1, "%s:\n", base_path);
	malloc_and_put_files(&files, dir, flags, base_path);
	if (flags->long_format)
		dprintf(1, "Total %lu\n", getBlockSize(&files));
	closedir(dir);
	while (files.files[++i])
	{
		if (strncmp(files.files[i], ".", 1) == 0 && !flags->all)
			continue ;
		if (flags->long_format)
		{
			getPerms(files.stats[i]);
			ft_dprintf(1, "%s", files.files[i]);
			if (S_ISLNK(files.stats[i].st_mode))
				getSymlink(files.stats[i], files.real_paths[i]);
			write(1, "\n", 1);
		}
		else if (flags->colors)
		{
			if (is_directory(files.real_paths[i]))
				dprintf(1, "\033[0;34m%s  \033[0m", files.files[i]);
			else
				ft_dprintf(1, "%s  ", files.files[i]);
		}
		else
			ft_dprintf(1, "%s  ", files.files[i]);
		count++;
	}
	if (count >= 1 && !flags->long_format)
		ft_dprintf(1, "\n");
	i = -1;
	while (files.files[++i])
	{
		if (strcmp(files.files[i], ".") == 0 || strcmp(files.files[i], "..") == 0)
		{
			free(files.files[i]);
			free(files.real_paths[i]);
			continue ;
		}
		if (is_directory(files.real_paths[i]) && !S_ISLNK(files.stats[i].st_mode))
		{
			if (flags->recursive && count >= 1)
				ft_dprintf(1, "\n");
			if (flags->recursive)
				recursive_ls(files.real_paths[i], flags);
		}
		free(files.files[i]);
		free(files.real_paths[i]);
	}
	i = 0;
	free(files.stats);
	free(files.real_paths);
	free(files.files);
}

int	do_flags(t_flags *flags)
{
	int i;
	int count;

	i = -1;
	count = 0;
	if (flags->recursive && flags->file_count >= 1)
	{
		while (flags->files[++i])
		{
			if (is_directory(flags->files[i]))
				continue ;
			ft_dprintf(1, "%s  ", flags->files[i]);
			count++;
		}
		if (count >= 1)
			ft_dprintf(1, "\n");
		i = -1;
		while (flags->files[++i])
		{
			if (!is_directory(flags->files[i]))
				continue ;
			if (count >= 1)
				ft_dprintf(1, "\n");
			recursive_ls(flags->files[i], flags);
			count++;
		}
	}
	else
	{
		while (flags->files[++i])
		{
			if (is_directory(flags->files[i]))
				continue ;
			ft_dprintf(1, "%s  ", flags->files[i]);
			count++;
		}
		if (count >= 1)
			ft_dprintf(1, "\n");
		i = -1;
		while (flags->files[++i])
		{
			if (!is_directory(flags->files[i]))
				continue ;
			if (count >= 1)
				ft_dprintf(1, "\n");
			recursive_ls(flags->files[i], flags);
			count++;
		}
	}
	return (1);
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
	i = 1;
	exit_code = 0;
	flags->file_count = 0;
	while (i < ac)
	{
		if (av[i][0] == '-')
			exit_help(check_flags_loop(av, flags, i), flags, flags->file_count);
		else
		{
			if (file_exists(av[i]) == false)
			{
				exit_code = 2;
				ft_putstr_fd(av[0], 2);
				ft_putstr_fd(": cannot access '", 2);
				ft_putstr_fd(av[i], 2);
				ft_putstr_fd("': No such file or directory\n", 2);
			}
			else
				flags->files[flags->file_count++] = ft_strdup(av[i]);
		}
		i++;
	}
	if (flags->file_count < 1 && exit_code == 0)
		flags->files[flags->file_count++] = ft_strdup(".");
	flags->files[flags->file_count] = NULL;
	quicksort(flags->files, NULL, NULL, 0, flags->file_count - 1);
	do_flags(flags);
	i = -1;
	while (flags->files[++i])
		free(flags->files[i]);
	free(flags->files);
	free(flags);
	return (exit_code);
}
