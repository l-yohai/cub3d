/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/06 09:10:27 by yohlee            #+#    #+#             */
/*   Updated: 2020/07/07 10:04:43 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	raycasting(t_info *info)
{
	int	x;

	x = 0;
	cast_floor_ceiling(info);
	while (x < info->width)
	{
		init_ray(info, &info->player, &info->ray, x);
		calculate_step_direction(&info->player, &info->ray);
		perform_dda(info, &info->ray);
		calculate_wall_distance(&info->player, &info->ray);
		calculate_wall_height(info, &info->ray);
		get_wall_texture(&info->player, &info->ray);
		get_wall_color(info, &info->ray, x);
		info->z_buffer[x] = info->ray.perp_wall_dist;
		x++;
	}
}

void	sprite_raycasting(t_info *info, t_player *player)
{
	int	i;
	int	stripe;

	sort_sprite(info, player);
	i = 0;
	while (i < info->num_sprite)
	{
		translate_sprite(info, player, &info->s_ray, i);
		calculate_sprite_height(info, &info->s_ray);
		calculate_sprite_width(info, &info->s_ray);
		stripe = info->s_ray.draw_start_x;
		while (stripe < info->s_ray.draw_end_x)
		{
			get_sprite_color(info, &info->s_ray, stripe);
			stripe++;
		}
		i++;
	}
}

int		main_loop(t_info *info)
{
	raycasting(info);
	sprite_raycasting(info, &info->player);
	draw(info);
	return (0);
}
