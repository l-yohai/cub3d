/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/02 03:15:28 by yohlee            #+#    #+#             */
/*   Updated: 2020/07/02 16:31:08 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "libft/libft.h"
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct	s_info
{
	int			width;
	int			height;

	int			**texture;
	char		*north_texture_path;
	char		*south_texture_path;
	char		*west_texture_path;
	char		*east_texture_path;
	char		*sprite_texture_path;

	int			floor_color;
	int			ceiling_color;
}				t_info;

int	error(t_info *info);

int	get_screen_size(t_info *info, char *line)
{
	int	i;

	i = 0;
	while (line[i] && (line[i] == 'R' || line[i] == ' '))
		i++;
	if (line[i] >= '1' && line[i] <= '9')
	{
		info->width = ft_atoi(&line[i]);
		i += ft_nbrlen(info->width);
	}
	else
		return (-1);
	while (line[i] && (line[i] == ' '))
		i++;
	if (line[i] >= '1' && line[i] <= '9')
	{
		info->height = ft_atoi(&line[i]);
		i += ft_nbrlen(info->height);
	}
	else
		return (-1);
	if (!info->width || !info->height)
		return (-1);
	return (1);
}

int	get_north_texture(t_info *info, char *line)
{
	if (!())
	info->north_texture_path
	return (1);
}

int	get_south_texture(t_info *info, char *line)
{
	return (1);
}

int	get_west_texture(t_info *info, char *line)
{
	return (1);
}

int	get_east_texture(t_info *info, char *line)
{
	return (1);
}

int	get_sprite_texture(t_info *info, char *line)
{
	return (1);
}

int	get_floor_color(t_info *info, char *line)
{
	return (1);
}

int	get_ceiling_color(t_info *info, char *line)
{
	return (1);
}

int	parse_line(t_info *info, char *line)
{
	if (ft_strncmp(line, "R", 1) == 0)
		return (get_screen_size(info, line));
	else if (ft_strncmp(line, "NO", 2) == 0)
		return (get_north_texture(info, line));
	else if (ft_strncmp(line, "SO", 2) == 0)
		return (get_south_texture(info, line));
	else if (ft_strncmp(line, "WE", 2) == 0)
		return (get_west_texture(info, line));
	else if (ft_strncmp(line, "EA", 2) == 0)
		return (get_east_texture(info, line));
	else if (ft_strncmp(line, "S", 1) == 0)
		return (get_sprite_texture(info, line));
	else if (ft_strncmp(line, "F", 1) == 0)
		return (get_floor_color(info, line));
	else if (ft_strncmp(line, "C", 1) == 0)
		return (get_ceiling_color(info, line));
	else
		return (0);
}

int	main(int argc, char **argv)
{
	int	fd;
	char	*line;
	int		ret;
	t_info	info;

	fd = open("map.cub", O_RDONLY);
	while ((ret = get_next_line(fd, &line)) > 0)
	{
		printf("%s %d\n", line, parse_line(&info, line));
		free(line);
	}
	ret = get_next_line(fd, &line);
	free(line);
}