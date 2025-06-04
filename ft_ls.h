/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 15:09:27 by etaquet           #+#    #+#             */
/*   Updated: 2025/06/04 22:14:45 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/stat.h>
# include <stdbool.h>
# include <sys/types.h>
# include <dirent.h>
# include <ctype.h>

typedef struct s_flags
{
	int		recursive;
	int		long_format;
	int		reverse;
	int		time_sort;
	int		all;
	int		colors;
	char	**files;
	int		file_count;
	char	*dir_name;
	bool	dir_flag;
}   t_flags;

void	quicksort(char *arr[], int low, int high);
int		is_directory(const char *base_path, const char *path);
int		is_executable_file(const char *path);
int		is_symlink(const char *path);
char	*get_real_path(const char *base_path, const char *path);
void	swap(char **a, char **b);
void	timesort(char *names[], char *realpaths[], int low, int high);
size_t	get_mtime(const char *path);
int		str_lower_cmp(const char *a, const char *b);

#endif