/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/01 11:15:43 by yohlee            #+#    #+#             */
/*   Updated: 2020/07/01 20:24:47 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_info(t_info *info)
{
	info->mlx = mlx_init();
	info->win = mlx_new_window(info->mlx, WIDTH, HEIGHT, "yohlee");
	info->img.img = mlx_new_image(info->mlx, WIDTH, HEIGHT);
	info->img.data = (int *)mlx_get_data_addr(\
		info->img.img, &info->img.bpp, &info->img.size_l, &info->img.endian);
}

void	init_player(t_player *player)
{
	player->x = 22.0;
	player->y = 11.5;
	player->dir_x = -1.0;
	player->dir_y = 0.0;
	player->plane_x = 0.0;
	player->plane_y = 0.66;
	player->move_speed = 0.05;
	player->rot_speed = 0.05;
}

void	init_buffer(t_info *info)
{
	int	i;
	int	j;

	i = 0;
	while (i < HEIGHT)
	{
		j = 0;
		while (j < WIDTH)
		{
			info->buf[i][j] = 0;
			j++;
		}
		i++;
	}
}

int		init_texture(t_info *info)
{
	int	i;
	int	j;

	if (!(info->texture = (int **)malloc(sizeof(int *) * 11)))
		return (-1);
	i = 0;
	while (i < 11)
	{
		if (!(info->texture[i] =\
				(int *)malloc(sizeof(int) * (TEX_HEIGHT * TEX_WIDTH))))
			return (-1);
		i++;
	}
	i = 0;
	while (i < 11)
	{
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
