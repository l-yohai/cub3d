/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/06 09:09:26 by yohlee            #+#    #+#             */
/*   Updated: 2020/07/07 07:26:31 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	sort_sprite(t_info *info, t_player *player)
{
	int	i;

	i = 0;
	while (i < info->num_sprite)
	{
		info->sprite[i].distance =\
			((player->x - info->sprite[i].x)\
				* (player->x - info->sprite[i].x)\
					+ (player->y - info->sprite[i].y)\
						* (player->y - info->sprite[i].y));
		i++;
	}
	sort(info);
}

void	translate_sprite(\
				t_info *info, t_player *player, t_sprite_ray *s_ray, int i)
{
	double	v_mode;

	v_mode = 0.0;
	s_ray->x = info->sprite[i].x - player->x;
	s_ray->y = info->sprite[i].y - player->y;
	s_ray->inversion_detection = 1.0 /\
		(player->plane_x * player->dir_y - player->dir_x * player->plane_y);
	s_ray->transform_x = s_ray->inversion_detection *\
		(player->dir_y * s_ray->x - player->dir_x * s_ray->y);
	s_ray->transform_y = s_ray->inversion_detection *\
		(-player->plane_y * s_ray->x + player->plane_x * s_ray->y);
	s_ray->screen_x = (int)((info->width / 2) *\
						(1 + s_ray->transform_x / s_ray->transform_y));
	s_ray->v_move_screen = (int)(v_mode / s_ray->transform_y);
}

void	calculate_sprite_height(t_info *info, t_sprite_ray *s_ray)
{
	int	v_div;

	v_div = 1;
	s_ray->height = (int)fabs((info->height / s_ray->transform_y) / v_div);
	s_ray->draw_start_y =\
		-s_ray->height / 2 + info->height / 2 + s_ray->v_move_screen;
	if (s_ray->draw_start_y < 0)
		s_ray->draw_start_y = 0;
	s_ray->draw_end_y =\
		s_ray->height / 2 + info->height / 2 + s_ray->v_move_screen;
	if (s_ray->draw_end_y >= info->height)
		s_ray->draw_end_y = info->height - 1;
}

void	calculate_sprite_width(t_info *info, t_sprite_ray *s_ray)
{
	int	u_div;

	u_div = 1;
	s_ray->width = (int)fabs((info->height / s_ray->transform_y) / u_div);
	s_ray->draw_start_x = -s_ray->width / 2 + s_ray->screen_x;
	if (s_ray->draw_start_x < 0)
		s_ray->draw_start_x = 0;
	s_ray->draw_end_x = s_ray->width / 2 + s_ray->screen_x;
	if (s_ray->draw_end_x >= info->width)
		s_ray->draw_end_x = info->width - 1;
}

void	get_sprite_color(t_info *info, t_sprite_ray *s_ray, int stripe)
{
	int	color;
	int	y;
	int	draw;

	s_ray->tex_x =\
		(int)((256 * (stripe - (-s_ray->width / 2 + s_ray->screen_x)) *\
			TEX_WIDTH / s_ray->width) / 256);
	if (s_ray->transform_y > 0 && stripe > 0 &&\
		stripe < info->width && s_ray->transform_y < info->z_buffer[stripe])
	{
		y = s_ray->draw_start_y;
		while (y < s_ray->draw_end_y)
		{
			draw = (y - s_ray->v_move_screen) * 256 -\
					info->height * 128 + s_ray->height * 128;
			s_ray->tex_y = ((draw * TEX_HEIGHT) / s_ray->height) / 256;
			color = info->texture[4][TEX_WIDTH * s_ray->tex_y + s_ray->tex_x];
			if ((color & 0x00FFFFFF) != 0)
				info->buf[y][stripe] = color;
			y++;
		}
	}
}
