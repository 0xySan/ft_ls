/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 15:04:49 by etaquet           #+#    #+#             */
/*   Updated: 2025/06/04 22:22:57 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <string.h>

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

void	ft_putstr_fd(const char *str, int fd)
{
	write(fd, str, ft_strlen(str));
}

void	put_message_help(const char *str)
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

void	malloc_and_put_files(char ***files, DIR *dir, t_flags *flags, const char *bp)
{
	int				count;
	struct dirent	*entry;
	char			**real_path;

	count = 0;
	while ((entry = readdir(dir)) != NULL)
	{
		if (strncmp(entry->d_name, ".", 1) == 0 && !flags->all)
			continue ;
		count++;
	}
	*files = malloc(sizeof(char *) * (count + 1));
	if (flags->time_sort)
			real_path = malloc(sizeof(char *) * (count + 1));
	rewinddir(dir);
	count = 0;
	while ((entry = readdir(dir)) != NULL)
	{
		if (strncmp(entry->d_name, ".", 1) == 0 && !flags->all)
			continue ;
		(*files)[count] = strdup(entry->d_name);
		if (flags->time_sort)
			real_path[count] = get_real_path(bp, entry->d_name);
		count++;
	}
	(*files)[count] = NULL;
	if (!flags->time_sort)
		quicksort(*files, 0, count - 1);
	else
	{
		real_path[count] = NULL;
		timesort(*files, real_path, 0, count - 1);
		int i = -1;
		while(real_path[++i])
			free(real_path[i]);
		free(real_path);
	}
	if (flags->reverse)
	{
		for (int i = 0; i < count / 2; i++)
		{
			char *tmp = (*files)[i];
			(*files)[i] = (*files)[count - 1 - i];
			(*files)[count - 1 - i] = tmp;
		}
	}
}

void recursive_ls(const char *base_path, t_flags *flags)
{
	DIR				*dir;
	struct stat		st;
	char			path[PATH_MAX];
	int				len;
	int				count;
	char			**files;
	int				i;

	count = 0;
	i = 0;
	dir = opendir(base_path);
	if (!dir)
		return ;
	if (flags->file_count - 1 >= 1 || flags->recursive)
		printf("%s:\n", base_path);
	malloc_and_put_files(&files, dir, flags, base_path);
	closedir(dir);
	while (files[i])
	{
		if (strncmp(files[i], ".", 1) == 0 && !flags->all)
			continue ;
		if (flags->colors)
		{
			if (is_directory(base_path, files[i]))
				printf("\033[0;34m%s  \033[0m", files[i]);
			else
				printf("%s  ", files[i]);
		}
		else
			printf("%s  ", files[i]);
		i++;
		count++;
	}
	if (count >= 1)
		printf("\n");
	i = 0;
	while (files[i])
	{
		if (strcmp(files[i], ".") == 0 || strcmp(files[i], "..") == 0)
		{
			i++;
			continue ;
		}
		len = strlen(base_path);
		strcpy(path, base_path);
		if (base_path[len - 1] != '/')
			strcat(path, "/");
		strcat(path, files[i]);
		if (stat(path, &st) == 0 && S_ISDIR(st.st_mode) && !is_symlink(path))
		{
			if (flags->recursive && count >= 1)
				printf("\n");
			if (flags->recursive)
				recursive_ls(path, flags);
		}
		i++;
	}
	i = 0;
	while (files[i])
		free(files[i++]);
	free(files);
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
			if (is_directory(NULL, flags->files[i]))
				continue ;
			printf("%s  ", flags->files[i]);
			count++;
		}
		if (count >= 1)
			printf("\n");
		i = -1;
		while (flags->files[++i])
		{
			if (!is_directory(NULL, flags->files[i]))
				continue ;
			if (count >= 1)
				printf("\n");
			recursive_ls(flags->files[i], flags);
			count++;
		}
	}
	else
	{
		while (flags->files[++i])
		{
			if (is_directory(NULL, flags->files[i]))
				continue ;
			printf("%s  ", flags->files[i]);
			count++;
		}
		if (count >= 1)
			printf("\n");
		i = -1;
		while (flags->files[++i])
		{
			if (!is_directory(NULL, flags->files[i]))
				continue ;
			if (count >= 1)
				printf("\n");
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
	quicksort(flags->files, 0, flags->file_count - 1);
	do_flags(flags);
	i = -1;
	while (flags->files[++i])
		free(flags->files[i]);
	free(flags->files);
	free(flags);
	return (exit_code);
}
