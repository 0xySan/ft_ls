/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quick_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 11:08:00 by etaquet           #+#    #+#             */
/*   Updated: 2026/03/26 22:26:55 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_ls.h"

void	swap(char **a, char **b)
{
	char	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

int	str_lower_cmp(const char *a, const char *b)
{
	while (*a && *b)
	{
		if ((unsigned char)*a != (unsigned char)*b)
			return ((unsigned char)*a - (unsigned char)*b);
		a++;
		b++;
	}
	return ((unsigned char)*a - (unsigned char)*b);
}

static int	partition(t_files *files, int low, int high)
{
	char	*pivot;
	int		i;
	int		j;

	pivot = files->files[high];
	i = low - 1;
	j = low - 1;
	while (++j < high)
	{
		if (str_lower_cmp(files->files[j], pivot) <= 0)
		{
			i++;
			swap(&files->files[i], &files->files[j]);
			swap(&files->real_paths[i], &files->real_paths[j]);
			swap_stat(&files->stats[i], &files->stats[j]);
		}
	}
	swap(&files->files[i + 1], &files->files[high]);
	swap(&files->real_paths[i + 1], &files->real_paths[high]);
	swap_stat(&files->stats[i + 1], &files->stats[high]);
	return (i + 1);
}

void	quicksort(t_files *files, int low, int high)
{
	int	pi;

	if (low < high)
	{
		pi = partition(files, low, high);
		quicksort(files, low, pi - 1);
		quicksort(files, pi + 1, high);
	}
}
