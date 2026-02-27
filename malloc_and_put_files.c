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

typedef struct s_dnode
{
	char			*name;
	struct s_dnode	*next;
}	t_dnode;

static t_dnode	*collect_entries(DIR *d, t_flags *f, int *count)
{
	struct dirent	*e;
	t_dnode			*head;
	t_dnode			*node;

	head = NULL;
	*count = 0;
	e = readdir(d);
	while (e)
	{
		if (!(e->d_name[0] == '.' && !f->all))
		{
			node = malloc(sizeof(t_dnode));
			if (!node)
				exit(EXIT_FAILURE);
			node->name = ft_strdup(e->d_name);
			node->next = head;
			head = node;
			(*count)++;
		}
		e = readdir(d);
	}
	return (head);
}

static void	free_list(t_dnode *lst)
{
	t_dnode	*tmp;

	while (lst)
	{
		tmp = lst->next;
		free(lst->name);
		free(lst);
		lst = tmp;
	}
}

static int	fill_from_list(t_files *t, t_dnode *lst,
	t_flags *f, const char *bp)
{
	int	i;

	i = 0;
	while (lst)
	{
		t->files[i] = lst->name;
		lst->name = NULL;
		t->real_paths[i] = get_real_path(bp, t->files[i]);
		if (!t->files[i] || !t->real_paths[i])
		{
			free(t->files[i]);
			free(t->real_paths[i]);
			lst = lst->next;
			continue ;
		}
		if (lstat(t->real_paths[i], &t->stats[i]) == -1)
		{
			ft_dprintf(2,
				"./ft_ls: cannot access '%s': No such file or directory\n",
				t->real_paths[i]);
			f->error_code = 1;
			free(t->files[i]);
			free(t->real_paths[i]);
			lst = lst->next;
			continue ;
		}
		i++;
		lst = lst->next;
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
	int		n;
	int		actual_count;
	t_dnode	*lst;

	lst = collect_entries(d, f, &n);
	t->files = malloc((n + 1) * sizeof(char *));
	t->real_paths = malloc((n + 1) * sizeof(char *));
	t->stats = malloc((n + 1) * sizeof(struct stat));
	if (!t->files || !t->real_paths || !t->stats)
		exit(EXIT_FAILURE);
	t->file_count = n;
	actual_count = fill_from_list(t, lst, f, bp);
	free_list(lst);
	if (f->not_sorted)
		reverse_entries(t, actual_count);
	else if (f->time_sort)
		timesort(t, 0, actual_count - 1);
	else
		quicksort(t, 0, actual_count - 1);
	if (f->reverse && !f->not_sorted)
		reverse_entries(t, actual_count);
}
