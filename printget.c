/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printget.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:41:33 by etaquet           #+#    #+#             */
/*   Updated: 2025/06/24 19:38:12 by etaquet          ###   ########.fr       */
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
	char		*timestr;

	timestr = ctime(&st.st_mtime);
	write(1, timestr + 4, 12);
	ft_dprintf(1, " ");
}
