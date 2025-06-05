/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_sort.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 12:38:59 by etaquet           #+#    #+#             */
/*   Updated: 2025/06/05 14:08:52 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	swap_stat(struct stat *a, struct stat *b)
{
	struct stat tmp = *a;
	*a = *b;
	*b = tmp;
}

void	timesort(char *names[], char *realpaths[], struct stat stats[], int low, int high)
{
	if (low < high)
	{
		size_t	pivot_time;
		char	*pivot_name;
		int		i;
		int		j;

		pivot_time = ((size_t)stats[high].st_mtim.tv_sec << 32)
			+ stats[high].st_mtim.tv_nsec;
		pivot_name = names[high];
		i = low - 1;
		j = low;

		while (j < high)
		{
			size_t	curr_time = ((size_t)stats[j].st_mtim.tv_sec << 32)
				+ stats[j].st_mtim.tv_nsec;

			if (curr_time > pivot_time ||
				(curr_time == pivot_time &&
					str_lower_cmp(names[j], pivot_name) < 0))
			{
				i++;
				swap(&names[i], &names[j]);
				swap(&realpaths[i], &realpaths[j]);
				swap_stat(&stats[i], &stats[j]);
			}
			j++;
		}
		swap(&names[i + 1], &names[high]);
		swap(&realpaths[i + 1], &realpaths[high]);
		swap_stat(&stats[i + 1], &stats[high]);

		timesort(names, realpaths, stats, low, i);
		timesort(names, realpaths, stats, i + 2, high);
	}
}