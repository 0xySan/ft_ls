/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printget.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:41:33 by etaquet           #+#    #+#             */
/*   Updated: 2025/07/15 19:08:51 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	putchar_perm(int cond, char c)
{
	if (cond)
		write(1, &c, 1);
	else
		ft_dprintf(1, "-");
}

void	print_file_type(mode_t mode)
{
	if ((mode & S_IFMT) == S_IFLNK)
		ft_dprintf(1, "l");
	else if ((mode & S_IFMT) == S_IFDIR)
		ft_dprintf(1, "d");
	else
		ft_dprintf(1, "-");
}

void	print_mod_time(struct stat st)
{
	char		*file_time;
	char		*file_time_copy;
	char		*curr_time;
	time_t		now;
	int			i;

	file_time = ctime(&st.st_mtime);
	file_time_copy = malloc(26);
	if (!file_time_copy)
		return ;
	i = -1;
	while (++i < 26)
		file_time_copy[i] = file_time[i];
	time(&now);
	curr_time = ctime(&now);
	if (file_time_copy[20] == curr_time[20] &&
		file_time_copy[21] == curr_time[21] &&
		file_time_copy[22] == curr_time[22] &&
		file_time_copy[23] == curr_time[23])
		write(1, file_time_copy + 4, 12);
	else
	{
		write(1, file_time_copy + 4, 6);
		write(1, "  ", 2);
		write(1, file_time_copy + 20, 4);
	}
	write(1, " ", 1);
	free(file_time_copy);
}
