/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 15:09:27 by etaquet           #+#    #+#             */
/*   Updated: 2026/04/07 16:00:19 by etaquet          ###   ########.fr       */
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
#if __has_include(<sys/acl.h>)
# include <sys/acl.h>
# include <acl/libacl.h>
#endif
# include <sys/xattr.h>
# include <sys/ioctl.h>
# include "ft_dprintf/ft_dprintf.h"
# include "libft/libft.h"

typedef struct s_flags
{
	int		recursive;
	int		long_format;
	int		owner;
	int		group;
	int 	directory;
	int		reverse;
	int		size;
	int		human_readable;
	int		time_sort;
	int		not_sorted;
	int		color;
	int		all;
	int		almost_all;
	int		author;
	int		width;
	int		one;
	int		hyperlink;
	int		file_type;
	int		classify;
	int		numeric;
	double	size_unit;
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
	int		blocks_w;
	int		acl_mark;
}	t_colwidths;

typedef struct s_dnode
{
	char			*name;
	struct s_dnode	*next;
}	t_dnode;

/* PARSING DIRECTORY */

void		getperms(struct stat st, const char *path, t_flags *flags, t_colwidths cw);
void		getsymlink(struct stat st, const char *path, int color);
void		do_flags(t_flags *flags);
void		init_flags(t_flags *flags);
void		exit_help(int code, t_flags *flags, int count);
char		*get_real_path(const char *base_path, const char *path);
int			is_symlink(const char *path);
int			is_directory(const char *path);
int			check_flags_loop(char **av, t_flags *flags, int i);
t_colwidths	init_colwidths(t_files *files, int group, int all,
				int almost_all, int numeric, int human, int show_blocks, double size_unit);

/* PRINTING DIRECTORY */

void		print_file_type(mode_t mode);
void		print_mod_time(struct stat st);
void		reverse_files(char **files, int n);
void		malloc_and_put_files(t_files *t, DIR *d, t_flags *f, const char *bp);
void		print_columns(char **nm, char **rp, int cnt, struct stat *st, t_flags *flags);

/* SORTING DIRECTORY */

void		timesort(t_files *files, int low, int high);
void		timesort_files(char **files, int low, int high);
void		quicksort(t_files *files, int low, int high);
void		quicksort_files(char **files, int low, int high);
void		swap(char **a, char **b);
void		swap_stat(struct stat *a, struct stat *b);
int			str_lower_cmp(const char *a, const char *b);

/* UTILS */

void		print_padded_num(unsigned long n, int width);
void		print_padded_str(const char *s, int width);
void		print_color_name(const char *name, const char *path,
				struct stat *st, int color);
void		recursive_ls(const char *base_path, t_flags *flags);
int			count_digits(unsigned long n);
int			count_files(char **av);
size_t		getblocksize(t_files *files);
bool		file_exists(char *filename);
char 		*get_size_human_readable(off_t size, double size_unit);
char		*getblocksize_human_readable(t_files *files, double size_unit);
char		*get_blocks_human_readable(size_t blocks, double size_unit);
void		print_block_prefix(struct stat st, int width, int human, double size_unit);
const char	*get_color_code(struct stat *st, const char *name);
const char	*get_orphan_link_color_code(void);

// check.c

int			check_width_arg(const char *arg, const char *argtwo, t_flags *flags, int *i);
int			check_hyperlink_arg(const char *arg, t_flags *flags, int *i);
int			check_color_arg(const char *arg, t_flags *flags, int *i);
// hyperlink.c
char		*build_hyperlink_path(const char *raw_path);
void		print_name_hyperlink(const char *name, struct stat *st, t_flags *flags, const char *path);
void		write_hyperlink_open(const char *hyperlink);
void		write_hyperlink_close(void);
void		print_normal_name(const char *name, const char *path,
				struct stat *st, t_flags *flags);
void		print_name_with_hyperlink(const char *name, const char *path, struct stat *st, t_flags *flags);
void		print_symlink_with_hyperlink(struct stat st, const char *path, int color, t_flags *flags, const char *hyperlink);

#endif
