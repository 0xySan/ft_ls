/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_sort.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 12:38:59 by etaquet           #+#    #+#             */
/*   Updated: 2025/06/04 22:39:55 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

size_t get_mtime(const char *path)
{
	struct stat st;
	if (lstat(path, &st) == 0)
		return (st.st_mtim.tv_sec << 32) + st.st_mtim.tv_nsec;
	return 0;
}

void timesort(char *names[], char *realpaths[], int low, int high)
{
	if (low < high)
	{
		size_t pivot_time = get_mtime(realpaths[high]);
		char *pivot_name = names[high];
		int i = low - 1;

		for (int j = low; j < high; j++) {
			size_t curr_time = get_mtime(realpaths[j]);
			if (curr_time > pivot_time ||
				(curr_time == pivot_time && str_lower_cmp(names[j], pivot_name) < 0))
			{
				i++;
				swap(&names[i], &names[j]);
				swap(&realpaths[i], &realpaths[j]);
			}
		}

		swap(&names[i + 1], &names[high]);
		swap(&realpaths[i + 1], &realpaths[high]);

		int pi = i + 1;
		timesort(names, realpaths, low, pi - 1);
		timesort(names, realpaths, pi + 1, high);
	}
}
