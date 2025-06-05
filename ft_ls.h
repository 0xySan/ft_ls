/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 15:09:27 by etaquet           #+#    #+#             */
/*   Updated: 2025/06/05 17:40:34 by etaquet          ###   ########.fr       */
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
# include <grp.h>
# include <pwd.h>
# include <time.h>
# include "ft_dprintf/ft_printf.h"

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

typedef struct s_files
{
	char		**files;
	char		**real_paths;
	struct stat	*stats;
	size_t		file_count;
}	t_files;

typedef struct s_uid_gid_cache {
    uid_t last_uid;
    gid_t last_gid;
    const char *pw_name;
    const char *gr_name;
} t_cache;

void	quicksort(char *arr[], char *rp[], struct stat stats[], int low, int high);
int		is_directory(const char *path);
int		is_executable_file(const char *path);
int		is_symlink(const char *path);
char	*get_real_path(const char *base_path, const char *path);
void	swap(char **a, char **b);
void	timesort(char *names[], char *realpaths[], struct stat stats[], int low, int high);
size_t	get_mtime(const char *path);
int		str_lower_cmp(const char *a, const char *b);
size_t	getBlockSize(t_files *files);
void	getPerms(struct stat st);
void	print_link_count(char *path);
void	swap_stat(struct stat *a, struct stat *b);
void	getSymlink(struct stat st, char *path);

#endif