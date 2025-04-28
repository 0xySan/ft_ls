/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 15:04:49 by etaquet           #+#    #+#             */
/*   Updated: 2025/04/21 16:06:05 by etaquet          ###   ########.fr       */
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

static int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
		return (c + 32);
	return (c);
}

static const char	*skip_punctuation(const char *s)
{
	while (*s && ((*s >= 0 && *s <= 47) || (*s >= 58 && *s <= 64) ||
		(*s >= 91 && *s <= 96) || (*s >= 123 && *s <= 127)))
		s++;
	return (s);
}

static int	ft_strcoll_like(const char *s1, const char *s2)
{
	s1 = skip_punctuation(s1);
	s2 = skip_punctuation(s2);
	while (*s1 && *s2)
	{
		if (ft_tolower(*s1) != ft_tolower(*s2))
			return (ft_tolower(*s1) - ft_tolower(*s2));
		s1++;
		s2++;
	}
	return (ft_tolower(*s1) - ft_tolower(*s2));
}

static void	ft_swap(char **a, char **b)
{
	char	*tmp = *a;
	*a = *b;
	*b = tmp;
}

static void	ft_sort_like_ls(char **arr, int count)
{
	int	i;
	int	j;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strcoll_like(arr[j], arr[j + 1]) > 0)
				ft_swap(&arr[j], &arr[j + 1]);
			j++;
		}
		i++;
	}
}

void ft_print_dir_entries(DIR *dir, t_flags *flags)
{
	struct dirent *entry;
	char **entries = NULL;
	int count = 0;
	int i;

	entry = readdir(dir);
	while (entry != NULL)
	{
		if (strncmp(entry->d_name, ".", 1) == 0 && flags->all == 0)
		{
			entry = readdir(dir);
			continue;
		}
		if (entry->d_type != DT_REG && entry->d_type != DT_DIR)
		{
			entry = readdir(dir);
			continue;
		}
		entries = realloc(entries, sizeof(char *) * (count + 1));
		entries[count++] = ft_strdup(entry->d_name);
		entry = readdir(dir);
	}
	ft_sort_like_ls(entries, count);
	i = 0;
	if (flags->reverse)
	{
		i = count - 1;
		while (i >= 0)
		{
			ft_putstr_fd(entries[i], 1);
			if (i - 1 >= 0)
				ft_putstr_fd(" ", 1);
			free(entries[i]);
			i--;
		}
		ft_putstr_fd("\n", 1);
		free(entries);
		return ;
	}
	while (i < count)
	{
		ft_putstr_fd(entries[i], 1);
		if (i + 1 < count)
			ft_putstr_fd(" ", 1);
		free(entries[i]);
		i++;
	}
	free(entries);
	ft_putstr_fd("\n", 1);
}

void init_flags(t_flags *flags)
{
	flags->recursive = 0;
	flags->long_format = 0;
	flags->reverse = 0;
	flags->time_sort = 0;
	flags->all = 0;
	flags->file_count = 0;
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

int print_files(int count, t_flags *flags)
{
	if (count > 0)
	{
		int z = 0;
		ft_sort_like_ls(flags->files, count);
		while (z < count)
		{
			printf("%s ", flags->files[z++]);
			free(flags->files[z - 1]);
		}
		printf("\n");
	}
	else
	{
		DIR *dir;

		dir = opendir(".");
		if (dir == NULL)
			return (1);
		ft_print_dir_entries(dir, flags);
		free(dir);
	}
	return (0);
}

int check_flags_loop(char **av, t_flags *flags, int i)
{
	int j;

	
	if (strcmp(av[i], "--help") == 0)
	{
		put_message_help(av[0]);
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

void exit_help(int code, t_flags *flags)
{
	(void)flags;
	if (code == 1)
		exit(1);
	else if (code == 2)
		exit(0);
}

int	main(int ac, char **av)
{
	int		i;
	int		exit_code;
	t_flags	*flags;
	int		count;

	flags = malloc(sizeof(t_flags));
	if (!flags)
		return (1);
	flags->files = malloc(sizeof(char *) * (count_files(av) + 1));
	init_flags(flags);
	i = 1;
	exit_code = 0;
	count = 0;
	while (i < ac)
	{
		if (av[i][0] == '-')
			exit_help(check_flags_loop(av, flags, i), flags);
		else
		{
			if (file_exists(av[i]) == false)
			{
				exit_code = 1;
				ft_putstr_fd(av[0], 2);
				ft_putstr_fd(": cannot access '", 2);
				ft_putstr_fd(av[i], 2);
				ft_putstr_fd("': No such file or directory\n", 2);
			}
			else
				flags->files[count++] = ft_strdup(av[i]);
		}
		i++;
	}
	print_files(count, flags);
	free(flags->files);
	free(flags);
	return (exit_code);
}
