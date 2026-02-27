/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_and_put_files.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 19:39:57 by etaquet           #+#    #+#             */
/*   Updated: 2025/06/24 19:42:11 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static int	count_entries(DIR *d, t_flags *f)
{
	struct dirent	*e;
	int				c;

	c = 0;
	e = readdir(d);
	while (e)
	{
		if (!(e->d_name[0] == '.' && !f->all))
			c++;
		e = readdir(d);
	}
	rewinddir(d);
	return (c);
}

static int	fill_entries(t_files *t, DIR *d, t_flags *f, const char *bp)
{
	struct dirent	*e;
	int				i;

	i = 0;
	e = readdir(d);
	while (e)
	{
		if (!(e->d_name[0] == '.' && !f->all))
		{
			t->files[i] = strdup(e->d_name);
			t->real_paths[i] = get_real_path(bp, e->d_name);
			if (!t->files[i] || !t->real_paths[i])
			{
				if (t->files[i])
					free(t->files[i]);
				if (t->real_paths[i])
					free(t->real_paths[i]);
				e = readdir(d);
				continue;
			}
			if (lstat(t->real_paths[i], &t->stats[i]) == -1)
			{
				free(t->files[i]);
				free(t->real_paths[i]);
				e = readdir(d);
				continue;
			}
			i++;
		}
		e = readdir(d);
	}
	t->files[i] = NULL;
	t->real_paths[i] = NULL;
	return (i);
}

static void	reverse_entries(t_files *t, int n)
{
	int	i;

	i = 0;
	while (i < n / 2)
	{
		swap(&t->files[i], &t->files[n - 1 - i]);
		swap(&t->real_paths[i], &t->real_paths[n - 1 - i]);
		swap_stat(&t->stats[i], &t->stats[n - 1 - i]);
		i++;
	}
}

void	malloc_and_put_files(t_files *t, DIR *d,
	t_flags *f, const char *bp)
{
	int	n;
	int	actual_count;

	n = count_entries(d, f);
	t->files = malloc((n + 1) * sizeof(char *));
	t->real_paths = malloc((n + 1) * sizeof(char *));
	t->stats = malloc(n * sizeof(struct stat));
	if (!t->files || !t->real_paths || !t->stats)
		exit(EXIT_FAILURE);
	actual_count = fill_entries(t, d, f, bp);
	if (f->time_sort)
		timesort(t, 0, actual_count - 1);
	else
		quicksort(t, 0, actual_count - 1);
	if (f->reverse)
		reverse_entries(t, actual_count);
}
