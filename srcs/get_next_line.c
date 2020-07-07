/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/02 16:35:52 by yohlee            #+#    #+#             */
/*   Updated: 2020/07/07 07:30:57 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/*
** return (1): line has been read.
** return (0): EOF has been reached.
** return (-1): an error happend.
** read without newline.
*/

char	*str_dup_cat(char *s1, char *s2)
{
	int		i;
	int		j;
	int		len;
	char	*res;

	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (NULL);
	len = ft_strlen(s1) + ft_strlen(s2);
	if (!(res = (char *)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	i = -1;
	while (s1[++i])
		res[i] = s1[i];
	free(s1);
	s1 = 0;
	j = -1;
	while (s2[++j])
		res[i + j] = s2[j];
	res[len] = '\0';
	return (res);
}

int		get_line(char *lines, char **line, char *found_chr)
{
	*line = ft_substr(lines, 0, found_chr - lines);
	++found_chr;
	ft_memmove(lines, found_chr, ft_strlen(found_chr) + 1);
	return (1);
}

char	*read_lines(int fd, char **line, char *lines)
{
	char		*found_chr;
	int			bytes;
	char		buf[BUFFER_SIZE + 1];

	while (((bytes = read(fd, buf, BUFFER_SIZE)) > 0))
	{
		buf[bytes] = '\0';
		lines = str_dup_cat(lines, buf);
		if ((found_chr = ft_strchr(lines, '\n')))
		{
			get_line(lines, line, found_chr);
			return (lines);
		}
	}
	if (lines)
	{
		*line = ft_strdup(lines);
		free(lines);
		lines = 0;
		return (0);
	}
	*line = ft_strdup("");
	return (0);
}

int		get_next_line(int fd, char **line)
{
	static char	*lines;
	char		*found_chr;

	if (!fd || fd > 65536 || !line || BUFFER_SIZE < 1 || read(fd, NULL, 0) < 0)
		return (-1);
	if (lines && (found_chr = ft_strchr(lines, '\n')))
		return (get_line(lines, line, found_chr));
	if ((lines = read_lines(fd, line, lines)))
		return (1);
	return (0);
}
