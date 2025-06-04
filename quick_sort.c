/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quick_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 11:08:00 by etaquet           #+#    #+#             */
/*   Updated: 2025/06/04 12:45:08 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	swap(char **a, char **b)
{
	char	*tmp;
	
	tmp = *a;
	*a = *b;
	*b = tmp;
}

int	str_lower_cmp(const char *a, const char *b)
{
	char	ca;
	char	cb;

	if (a[0] == '.')
		a++;
	if (b[0] == '.')
		b++;
	while (*a && *b)
	{
		ca = tolower((unsigned char)*a);
		cb = tolower((unsigned char)*b);
		if (ca != cb)
			return ca - cb;
		a++;
		b++;
	}
	return (tolower((unsigned char)*a) - tolower((unsigned char)*b));
}

int	partition(char *arr[], int low, int high)
{
	char	*pivot;
	int		i;
	int		j;

	pivot = arr[high];
	i = low - 1;
	j = low - 1;
	while (++j < high)
	{
		if (str_lower_cmp(arr[j], pivot) <= 0)
		{
			i++;
			swap(&arr[i], &arr[j]);
		}
	}
	swap(&arr[i + 1], &arr[high]);
	return (i + 1);
}

void	quicksort(char *arr[], int low, int high)
{
	int	pi;

	if (low < high)
	{
		pi = partition(arr, low, high);
		quicksort(arr, low, pi - 1);
		quicksort(arr, pi + 1, high);
	}
}