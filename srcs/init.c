/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/01 11:15:43 by yohlee            #+#    #+#             */
/*   Updated: 2020/07/07 10:59:19 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_info(t_info *info)
{
	info->width = 0;
	info->height = 0;
	info->map_width = 0;
	info->map_height = 0;
	info->map = NULL;
	info->buf = NULL;
	info->z_buffer = NULL;
	info->north_texture_path = NULL;
	info->south_texture_path = NULL;
	info->west_texture_path = NULL;
	info->east_texture_path = NULL;
	info->sprite_texture_path = NULL;
	info->floor_color = 0;
	info->ceiling_color = 0;
	info->num_sprite = 0;
	info->sprite = NULL;
}

void	init_player(t_player *player)
{
	player->x = 2;
	player->y = 5;
	player->dir_x = -1.0;
	player->dir_y = 0.0;
	player->plane_x = 0.0;
	player->plane_y = 0.66;
	player->move_speed = 0.05;
	player->rot_speed = 0.05;
}

int		init_buffer(t_info *info)
{
	int	i;

	i = 0;
	if (!(info->buf = ft_calloc(info->height, sizeof(int **))))
		return (exit_error(info));
	while (i < info->height)
	{
		if (!(info->buf[i] = ft_calloc(info->width, sizeof(int *))))
			return (exit_error(info));
		i++;
	}
	if (!(info->z_buffer = ft_calloc(info->width, sizeof(double *))))
		return (exit_error(info));
	return (1);
}

int		init_texture(t_info *info)
{
	int	i;
	int	j;

	if (!(info->texture = (int **)malloc(sizeof(int *) * 5)))
		return (-1);
	i = 0;
	while (i < 5)
	{
		if (!(info->texture[i] =\
				(int *)malloc(sizeof(int) * (TEX_HEIGHT * TEX_WIDTH))))
			return (-1);
		j = 0;
		while (j < TEX_HEIGHT * TEX_WIDTH)
		{
			info->texture[i][j] = 0;
			j++;
		}
		i++;
	}
	return (1);
}

int		init_map(t_info *info, t_list *lst)
{
	int	i;
	int	j;

	info->map_height = ft_lstsize(lst);
	if (!(info->map = (char **)malloc(sizeof(char *) * (info->map_height))))
		return (exit_error(info));
	i = 0;
	info->map_width = get_max_line_size(lst);
	while (lst)
	{
		if (!(info->map[i] =\
				(char *)malloc(sizeof(char) * (info->map_width + 1))))
			return (exit_error(info));
		j = 0;
		while (j++ < (int)ft_strlen(lst->content))
			info->map[i][j] = 0;
		while (j++ < info->map_width)
			info->map[i][j] = ' ';
		info->map[i][info->map_width] = '\0';
		i++;
		lst = lst->next;
	}
	return (1);
}
