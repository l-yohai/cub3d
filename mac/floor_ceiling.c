/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor_ceiling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/01 20:21:16 by yohlee            #+#    #+#             */
/*   Updated: 2020/07/02 16:06:45 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	get_floor_ceiling(t_player *player, t_floor_ceiling *fc)
{
	fc->tex_x =\
		(int)((fc->floor_x - (int)fc->floor_x) * TEX_WIDTH) & (TEX_WIDTH - 1);
	fc->tex_y =\
		(int)((fc->floor_y - (int)fc->floor_y) * TEX_HEIGHT) & (TEX_HEIGHT - 1);
	fc->floor_x += fc->row_distance * (2 * player->plane_x) / WIDTH;
	fc->floor_y += fc->row_distance * (2 * player->plane_y) / WIDTH;
	fc->checker_board_pattern = (int)((int)fc->floor_x + (int)fc->floor_y) & 1;
	if (fc->checker_board_pattern == 0)
		fc->floor_texture = 3;
	else
		fc->floor_texture = 4;
	fc->ceiling_texture = 6;
}

void	cast_floor_ceiling(t_info *info, t_player *player, t_floor_ceiling *fc)
{
	int	y;
	int	x;
	int	color;

	y = HEIGHT / 2 + 1;
	while (++y < HEIGHT)
	{
		fc->row_distance = (0.5 * HEIGHT) / (y - HEIGHT / 2);
		fc->floor_x =\
			player->x + fc->row_distance * (player->dir_x - player->plane_x);
		fc->floor_y =\
			player->y + fc->row_distance * (player->dir_y - player->plane_y);
		x = 0;
		while (++x < WIDTH)
		{
			get_floor_ceiling(player, fc);
			color = info->texture[fc->floor_texture]\
								[TEX_WIDTH * fc->tex_y + fc->tex_x];
			info->buf[y][x] = (color >> 1) & 8355711;
			color = info->texture[fc->ceiling_texture]\
								[TEX_WIDTH * fc->tex_y + fc->tex_x];
			info->buf[HEIGHT - y - 1][x] = (color >> 1) & 8355711;
		}
	}
}
