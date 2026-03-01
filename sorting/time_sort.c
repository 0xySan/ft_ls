/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_sort.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 12:38:59 by etaquet           #+#    #+#             */
/*   Updated: 2025/06/24 19:37:58 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_ls.h"

void	swap_stat(struct stat *a, struct stat *b)
{
	struct stat	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static size_t	get_mtime(const t_files *f, int idx)
{
	return (((size_t)f->stats[idx].st_mtim.tv_sec << 32)
		+ (size_t)f->stats[idx].st_mtim.tv_nsec);
}

static void	swap_entries(t_files *f, int a, int b)
{
	swap(&f->files[a], &f->files[b]);
	swap(&f->real_paths[a], &f->real_paths[b]);
	swap_stat(&f->stats[a], &f->stats[b]);
}

static int	partition_times(t_files *f, int low, int high)
{
	size_t	pivot;
	char	*name;
	int		i;
	int		j;

	pivot = get_mtime(f, high);
	name = f->files[high];
	i = low - 1;
	j = low;
	while (j < high)
	{
		if (get_mtime(f, j) > pivot
			|| (get_mtime(f, j) == pivot
				&& str_lower_cmp(f->files[j], name) < 0))
			swap_entries(f, ++i, j);
		j++;
	}
	swap_entries(f, i + 1, high);
	return (i + 1);
}

void	timesort(t_files *files, int low, int high)
{
	int	piv;

	if (low < high)
	{
		piv = partition_times(files, low, high);
		timesort(files, low, piv - 1);
		timesort(files, piv + 1, high);
	}
}
