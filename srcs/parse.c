/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/02 03:15:28 by yohlee            #+#    #+#             */
/*   Updated: 2020/07/07 07:00:02 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	get_screen_size(t_info *info, char *line)
{
	char	**size;

	if (!ft_isdigit(line[0]))
		return (exit_error(info));
	if (!(size = ft_split(line, ' ')))
		return (exit_error(info));
	if (!size || !size[0] || !size[1])
		return (exit_error(info));
	info->width = ft_atoi(size[0]);
	info->height = ft_atoi(size[1]);
	ft_free_2d(size);
	if (!info->width || !info->height)
		return (exit_error(info));
	return (1);
}

int	get_texture(t_info *info, char *line, char *direction)
{
	if (direction[0] == 'N' && direction[1] == 'O')
		info->north_texture_path = ft_strdup(line);
	else if (direction[0] == 'S' && direction[1] == 'O')
		info->south_texture_path = ft_strdup(line);
	else if (direction[0] == 'W' && direction[1] == 'E')
		info->west_texture_path = ft_strdup(line);
	else if (direction[0] == 'E' && direction[1] == 'A')
		info->east_texture_path = ft_strdup(line);
	else if (direction[0] == 'S' && direction[1] == '\0')
		info->sprite_texture_path = ft_strdup(line);
	else
		return (exit_error(info));
	return (1);
}

int	get_color(t_info *info, char *line, int direction)
{
	char	**rgb;
	int		color;
	int		i;

	if (!ft_isdigit(line[0]))
		return (exit_error(info));
	if (!(rgb = ft_split(line, ',')))
		return (exit_error(info));
	if (!rgb || !rgb[0] || !rgb[1] || !rgb[2])
		return (exit_error(info));
	color = 0;
	i = 0;
	while (rgb[i])
	{
		color *= 256;
		color += ft_atoi(rgb[i]);
		i++;
	}
	ft_free_2d(rgb);
	rgb = 0;
	if (direction == 'F')
		info->floor_color = color;
	else if (direction == 'C')
		info->ceiling_color = color;
	return (1);
}

int	parse_line(t_info *info, char *line)
{
	if (ft_strncmp(line, "R ", 2) == 0)
		return (get_screen_size(info, line + 2));
	else if (ft_strncmp(line, "NO ", 3) == 0)
		return (get_texture(info, line + 3, "NO"));
	else if (ft_strncmp(line, "SO ", 3) == 0)
		return (get_texture(info, line + 3, "SO"));
	else if (ft_strncmp(line, "WE ", 3) == 0)
		return (get_texture(info, line + 3, "WE"));
	else if (ft_strncmp(line, "EA ", 3) == 0)
		return (get_texture(info, line + 3, "EA"));
	else if (ft_strncmp(line, "S ", 2) == 0)
		return (get_texture(info, line + 2, "S"));
	else if (ft_strncmp(line, "F ", 2) == 0)
		return (get_color(info, line + 2, 'F'));
	else if (ft_strncmp(line, "C ", 2) == 0)
		return (get_color(info, line + 2, 'C'));
	else if (line[0] == '\0')
		return (1);
	return (0);
}

int	parse_cub(t_info *info, char *path)
{
	char	*line;
	int		ret;

	init_info(info);
	info->fd = open(path, O_RDONLY);
	while (get_next_line(info->fd, &line) > 0)
	{
		if ((ret = parse_line(info, line)) == -1)
			return (exit_error(info));
		if (ret == 0)
			break ;
		free(line);
	}
	if (!read_map(info, line))
		return (exit_error(info));
	if (!check_init(info))
		return (exit_error(info));
	if (!set_sprite(info))
		return (exit_error(info));
	return (1);
}
