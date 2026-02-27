/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 12:01:42 by etaquet           #+#    #+#             */
/*   Updated: 2025/06/24 18:43:10 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <grp.h>

char *get_size_human_readable(off_t size)
{
    const char	*units[] = {"", "K", "M", "G", "T", "P"};
    int			i;
    static char	buf[32];
    int			len;
    double		fraction;
    int			whole;
    int			decimal;

    i = 0;
    if (size == 0)
    {
        ft_strcpy(buf, "0");
        return (buf);
    }
    fraction = (double)size;
    while (fraction >= 1024.0 && i < 5)
    {
        fraction /= 1024.0;
        i++;
    }
    whole = (int)fraction;
    {
        double tenths = (fraction - whole) * 10.0;
        decimal = (int)tenths;
        if (tenths - decimal > 0.001)
            decimal++;
    }
    if (decimal >= 10)
    {
        whole++;
        decimal = 0;
    }
    if (whole < 10 && i > 0)
    {
        len = ft_strlen(ft_itoa(whole));
        ft_strcpy(buf, ft_itoa(whole));
        buf[len] = '.';
        ft_strcpy(buf + len + 1, ft_itoa(decimal));
        ft_strcpy(buf + len + 2, units[i]);
    }
    else
    {
        len = ft_strlen(ft_itoa(whole));
        ft_strcpy(buf, ft_itoa(whole));
        ft_strcpy(buf + len, units[i]);
    }
    return (buf);
}

char	*getblocksize_human_readable(t_files *files)
{
	size_t		blocksize;
	const char	*units[] = {"K", "M", "G", "T", "P"};
	int			i;
	static char	buf[32];
	double		fraction;
	int			whole;
	int			decimal;
	int			len;

	blocksize = getblocksize(files);
	if (blocksize == 0)
	{
		ft_strcpy(buf, "0");
		return (buf);
	}
	i = 0;
	fraction = (double)blocksize;
	while (fraction >= 1024.0 && i < 4)
	{
		fraction /= 1024.0;
		i++;
	}
	whole = (int)fraction;
	{
		double tenths = (fraction - whole) * 10.0;
		decimal = (int)tenths;
		if (tenths - decimal > 0.001)
			decimal++;
	}
	if (decimal >= 10)
	{
		whole++;
		decimal = 0;
	}
	if (whole < 10)
	{
		len = ft_strlen(ft_itoa(whole));
		ft_strcpy(buf, ft_itoa(whole));
		buf[len] = '.';
		ft_strcpy(buf + len + 1, ft_itoa(decimal));
		ft_strcpy(buf + len + 2, units[i]);
	}
	else
	{
		len = ft_strlen(ft_itoa(whole));
		ft_strcpy(buf, ft_itoa(whole));
		ft_strcpy(buf + len, units[i]);
	}
	return (buf);
}

size_t	getblocksize(t_files *files)
{
	int		i;
	size_t	total_count;

	total_count = 0;
	i = 0;
	while (files->files[i])
	{
		total_count += files->stats[i].st_blocks;
		i++;
	}
	return (total_count / 2);
}

void	getsymlink(struct stat st, char *path, int color)
{
	char		target[PATH_MAX];
	ssize_t		len;
	struct stat	st_target;
	const char	*cc;

	if (S_ISLNK(st.st_mode))
	{
		len = readlink(path, target, PATH_MAX - 1);
		if (len > 0 && len < PATH_MAX)
		{
			target[len] = '\0';
			buf_write(1, " -> ", 4);
			cc = NULL;
			if (color && stat(path, &st_target) == 0)
				cc = get_color_code(&st_target);
			if (cc)
			{
				buf_write(1, cc, ft_strlen(cc));
				buf_write(1, target, len);
				buf_write(1, "\033[0m", 4);
			}
			else
				buf_write(1, target, len);
		}
	}
}

static int	has_user_xattr(const char *path)
{
	char	buf[4096];
	ssize_t	len;
	int		i;

	len = listxattr(path, buf, sizeof(buf));
	if (len <= 0)
		return (0);
	i = 0;
	while (i < len)
	{
		if (strncmp(buf + i, "system.", 7) != 0
			&& strncmp(buf + i, "security.", 9) != 0)
			return (1);
		i += ft_strlen(buf + i) + 1;
	}
	return (0);
}

static void	print_acl_xattr(const char *path)
{
	acl_t	acl;

	acl = acl_get_file(path, ACL_TYPE_ACCESS);
	if (acl)
	{
		if (acl_entries(acl) > 3)
		{
			buf_write(1, "+", 1);
			acl_free(acl);
			return ;
		}
		acl_free(acl);
	}
	if (has_user_xattr(path))
		buf_write(1, "@", 1);
	else
		buf_write(1, " ", 1);
}

static void	update_cw(t_colwidths *cw, struct stat st, int group, int human)
{
    int				tmp;
    static uid_t	lu = (uid_t)-1;
    static gid_t	lg = (gid_t)-1;
    static int		pw_len;
    static int		gr_len;

    tmp = count_digits((unsigned long)st.st_nlink);
    if (tmp > cw->nlink_w)
        cw->nlink_w = tmp;
    if (human)
        tmp = ft_strlen(get_size_human_readable(st.st_size));
    else
        tmp = count_digits((unsigned long)st.st_size);
    if (tmp > cw->size_w)
        cw->size_w = tmp;
    if (st.st_gid != lg)
    {
        struct group *gr = getgrgid(st.st_gid);
        gr_len = gr ? ft_strlen(gr->gr_name) : 1;
        lg = st.st_gid;
    }
    if (gr_len > cw->group_w)
        cw->group_w = gr_len;
    if (!group)
    {
        if (st.st_uid != lu)
        {
            struct passwd *pw = getpwuid(st.st_uid);
            pw_len = pw ? ft_strlen(pw->pw_name) : 1;
            lu = st.st_uid;
        }
        if (pw_len > cw->owner_w)
            cw->owner_w = pw_len;
    }
}

t_colwidths	init_colwidths(t_files *files, int group, int all, int human)
{
    t_colwidths	cw;
    int			i;

    cw = (t_colwidths){0, 0, 0, 0};
    i = -1;
    while (files->files[++i])
    {
        if (!files->files[i] || !files->real_paths[i])
            continue ;
        if (strncmp(files->files[i], ".", 1) == 0 && !all)
            continue ;
        update_cw(&cw, files->stats[i], group, human);
    }
    return (cw);
}

static void	write_permbits(mode_t m)
{
	char	p[10];

	p[0] = (m & S_IRUSR) ? 'r' : '-';
	p[1] = (m & S_IWUSR) ? 'w' : '-';
	if (m & S_IXUSR)
		p[2] = (m & S_ISUID) ? 's' : 'x';
	else
		p[2] = (m & S_ISUID) ? 'S' : '-';
	p[3] = (m & S_IRGRP) ? 'r' : '-';
	p[4] = (m & S_IWGRP) ? 'w' : '-';
	if (m & S_IXGRP)
		p[5] = (m & S_ISGID) ? 's' : 'x';
	else
		p[5] = (m & S_ISGID) ? 'S' : '-';
	p[6] = (m & S_IROTH) ? 'r' : '-';
	p[7] = (m & S_IWOTH) ? 'w' : '-';
	if (m & S_IXOTH)
		p[8] = (m & S_ISVTX) ? 't' : 'x';
	else
		p[8] = (m & S_ISVTX) ? 'T' : '-';
	buf_write(1, p, 9);
}

void	getperms(struct stat st, const char *path, t_flags *flags, t_colwidths cw)
{
	static uid_t	last_uid = (uid_t)-1;
	static gid_t	last_gid = (gid_t)-1;
	static char		pw_buf[256];
	static char		gr_buf[256];
	struct passwd	*pw;
	struct group	*gr;

	print_file_type(st.st_mode);
	write_permbits(st.st_mode);
	print_acl_xattr(path);
	if (st.st_uid != last_uid)
	{
		pw = getpwuid(st.st_uid);
		if (pw)
		{
			int j = 0;
			while (pw->pw_name[j] && j < 255)
			{
				pw_buf[j] = pw->pw_name[j];
				j++;
			}
			pw_buf[j] = '\0';
		}
		else
			pw_buf[0] = '?', pw_buf[1] = '\0';
		last_uid = st.st_uid;
	}
	if (st.st_gid != last_gid)
	{
		gr = getgrgid(st.st_gid);
		if (gr)
		{
			int j = 0;
			while (gr->gr_name[j] && j < 255)
			{
				gr_buf[j] = gr->gr_name[j];
				j++;
			}
			gr_buf[j] = '\0';
		}
		else
			gr_buf[0] = '?', gr_buf[1] = '\0';
		last_gid = st.st_gid;
	}
	print_padded_num((unsigned long)st.st_nlink, cw.nlink_w);
	buf_write(1, " ", 1);
	if (!flags->group)
	{
		print_padded_str(pw_buf, cw.owner_w);
		buf_write(1, " ", 1);
	}
	print_padded_str(gr_buf, cw.group_w);
	buf_write(1, " ", 1);
	if (flags->human_readable)
	{
		char *hr = get_size_human_readable(st.st_size);
		int pad = cw.size_w - ft_strlen(hr);
		while (pad-- > 0)
			buf_write(1, " ", 1);
		buf_write(1, hr, ft_strlen(hr));
	}
	else
		print_padded_num((unsigned long)st.st_size, cw.size_w);
	buf_write(1, " ", 1);
	print_mod_time(st);
}
