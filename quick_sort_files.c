/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quick_sort_files.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 19:33:12 by etaquet           #+#    #+#             */
/*   Updated: 2025/06/24 19:35:46 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static int	partition(char **files, int low, int high)
{
	char	*pivot;
	int		i;
	int		j;

	pivot = files[high];
	i = low - 1;
	j = low - 1;
	while (++j < high)
	{
		if (str_lower_cmp(files[j], pivot) <= 0)
		{
			i++;
			swap(&files[i], &files[j]);
		}
	}
	swap(&files[i + 1], &files[high]);
	return (i + 1);
}

void	quicksort_files(char **files, int low, int high)
{
	int	pi;

	if (low < high)
	{
		pi = partition(files, low, high);
		quicksort_files(files, low, pi - 1);
		quicksort_files(files, pi + 1, high);
	}
}
