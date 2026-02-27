/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 15:09:27 by etaquet           #+#    #+#             */
/*   Updated: 2025/06/24 20:17:29 by etaquet          ###   ########.fr       */
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
# include <sys/acl.h>
# include <acl/libacl.h>
# include <sys/xattr.h>
# include <sys/ioctl.h>
# include "ft_dprintf/ft_dprintf.h"

typedef struct s_flags
{
	int		recursive;
	int		long_format;
	int		group;
	int 	directory;
	int		reverse;
	int		time_sort;
	int		not_sorted;
	int		color;
	int		all;
	char	**files;
	int		file_count;
	char	*dir_name;
	int		error_code;
	int		last_code;
	bool	dir_flag;
}	t_flags;

typedef struct s_files
{
	char		**files;
	char		**real_paths;
	struct stat	*stats;
	size_t		file_count;
}	t_files;

typedef struct s_uid_gid_cache
{
	uid_t		last_uid;
	gid_t		last_gid;
	const char	*pw_name;
	const char	*gr_name;
}	t_cache;

typedef struct s_colwidths
{
	int		nlink_w;
	int		owner_w;
	int		group_w;
	int		size_w;
}	t_colwidths;

void	quicksort(t_files *files, int low, int high);
int		is_directory(const char *path);
int		is_executable_file(const char *path);
int		is_symlink(const char *path);
char	*get_real_path(const char *base_path, const char *path);
void	swap(char **a, char **b);
void	timesort(t_files *files, int low, int high);
size_t	getblocksize(t_files *files);
void	getperms(struct stat st, const char *path, int group, t_colwidths cw);
void	swap_stat(struct stat *a, struct stat *b);
void	print_columns(char **names, int count);
void	getsymlink(struct stat st, char *path);
char	*ft_strdup(const char *s1);
int		ft_strlen(const char *str);
bool	file_exists(char *filename);
void	print_mod_time(struct stat st);
void	print_file_type(mode_t mode);
void	putchar_perm(int cond, char c);
int		count_files(char **av);
void	recursive_ls(const char *base_path, t_flags *flags);
void	malloc_and_put_files(t_files *t, DIR *d,
			t_flags *f, const char *bp);
void	exit_help(int code, t_flags *flags, int count);
int		check_flags_loop(char **av, t_flags *flags, int i);
int		check_flags(char **av, t_flags *flags, int i, int j);
void	init_flags(t_flags *flags);
void	put_message_help(char *str);
void	timesort_files(char **files, int low, int high);
void	quicksort_files(char **files, int low, int high);
int		str_lower_cmp(const char *a, const char *b);
int		count_digits(unsigned long n);
void	print_padded_num(unsigned long n, int width);
void	print_padded_str(const char *s, int width);
t_colwidths	init_colwidths(t_files *files, int group, int all);

#endif