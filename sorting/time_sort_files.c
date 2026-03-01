/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_sort_files.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 19:30:44 by etaquet           #+#    #+#             */
/*   Updated: 2025/06/24 19:31:17 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_ls.h"

static size_t	get_mtime(const char *path)
{
	struct stat	st;

	if (lstat(path, &st) == -1)
		return (0);
	return (((size_t)st.st_mtim.tv_sec << 32) + (size_t)st.st_mtim.tv_nsec);
}

static int	partition_times(char **files, int low, int high)
{
	size_t	pivot_time;
	char	*pivot_name;
	size_t	curr_time;
	int		i;
	int		j;

	pivot_time = get_mtime(files[high]);
	pivot_name = files[high];
	i = low - 1;
	j = low;
	while (j < high)
	{
		curr_time = get_mtime(files[j]);
		if (curr_time > pivot_time
			|| (curr_time == pivot_time
				&& str_lower_cmp(files[j], pivot_name) < 0))
		{
			i++;
			swap(&files[i], &files[j]);
		}
		j++;
	}
	swap(&files[i + 1], &files[high]);
	return (i + 1);
}

void	timesort_files(char **files, int low, int high)
{
	int	pivot_index;

	if (low < high)
	{
		pivot_index = partition_times(files, low, high);
		timesort_files(files, low, pivot_index - 1);
		timesort_files(files, pivot_index + 1, high);
	}
}
