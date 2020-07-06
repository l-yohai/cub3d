/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/01 17:09:52 by yohlee            #+#    #+#             */
/*   Updated: 2020/07/05 23:35:33 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "mlx/mlx.h"
#include <string.h>

t_sprite	sprite[NUM_SPRITES] =
{
	{20.5, 11.5, 10},

	{18.5,4.5, 10},
	{10.0,4.5, 10},
	{10.0,12.5,10},
	{3.5, 6.5, 10},
	{3.5, 20.5,10},
	{3.5, 14.5,10},
	{14.5,20.5,10},

	{18.5, 10.5, 9},
	{18.5, 11.5, 9},
	{18.5, 12.5, 9},

	{21.5, 1.5, 8},
	{15.5, 1.5, 8},
	{16.0, 1.8, 8},
	{16.2, 1.2, 8},
	{3.5,  2.5, 8},
	{9.5, 15.5, 8},
	{10.0, 15.1,8},
	{10.5, 15.8,8},
};

int	map[MAP_WIDTH][MAP_HEIGHT] =
{
	{8,8,8,8,8,8,8,8,8,8,8,4,4,6,4,4,6,4,6,4,4,4,6,4},
	{8,0,0,0,0,0,0,0,0,0,8,4,0,0,0,0,0,0,0,0,0,0,0,4},
	{8,0,3,3,0,0,0,0,0,8,8,4,0,0,0,0,0,0,0,0,0,0,0,6},
	{8,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6},
	{8,0,3,3,0,0,0,0,0,8,8,4,0,0,0,0,0,0,0,0,0,0,0,4},
	{8,0,0,0,0,0,0,0,0,0,8,4,0,0,0,0,0,6,6,6,0,6,4,6},
	{8,8,8,8,0,8,8,8,8,8,8,4,4,4,4,4,4,6,0,0,0,0,0,6},
	{7,7,7,7,0,7,7,7,7,0,8,0,8,0,8,0,8,4,0,4,0,6,0,6},
	{7,7,0,0,0,0,0,0,7,8,0,8,0,8,0,8,8,6,0,0,0,0,0,6},
	{7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,6,0,0,0,0,0,4},
	{7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,6,0,6,0,6,0,6},
	{7,7,0,0,0,0,0,0,7,8,0,8,0,8,0,8,8,6,4,6,0,6,6,6},
	{7,7,7,7,0,7,7,7,7,8,8,4,0,6,8,4,8,3,3,3,0,3,3,3},
	{2,2,2,2,0,2,2,2,2,4,6,4,0,0,6,0,6,3,0,0,0,0,0,3},
	{2,2,0,0,0,0,0,2,2,4,0,0,0,0,0,0,4,3,0,0,0,0,0,3},
	{2,0,0,0,0,0,0,0,2,4,0,0,0,0,0,0,4,3,0,0,0,0,0,3},
	{1,0,0,0,0,0,0,0,1,4,4,4,4,4,6,0,6,3,3,0,0,0,3,3},
	{2,0,0,0,0,0,0,0,2,2,2,1,2,2,2,6,6,0,0,5,0,5,0,5},
	{2,2,0,0,0,0,0,2,2,2,0,0,0,2,2,0,5,0,5,0,0,0,5,5},
	{2,0,0,0,0,0,0,0,2,0,0,0,0,0,2,5,0,5,0,5,0,5,0,5},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5},
	{2,0,0,0,0,0,0,0,2,0,0,0,0,0,2,5,0,5,0,5,0,5,0,5},
	{2,2,0,0,0,0,0,2,2,2,0,0,0,2,2,0,5,0,5,0,0,0,5,5},
	{2,2,2,2,1,2,2,2,2,2,2,1,2,2,2,5,5,5,5,5,5,5,5,5}
};

void	key_press_up(t_player *player)
{
	if (!map[(int)(player->x +\
					player->dir_x * player->move_speed)][(int)player->y])
		player->x += player->dir_x * player->move_speed;
	if (!map[(int)player->x][(int)(player->y +\
									player->dir_y * player->move_speed)])
		player->y += player->dir_y * player->move_speed;
}

void	key_press_down(t_player *player)
{
	if (!map[(int)(player->x -\
					player->dir_x * player->move_speed)][(int)player->y])
		player->x -= player->dir_x * player->move_speed;
	if (!map[(int)player->x][(int)(player->y -\
									player->dir_y * player->move_speed)])
		player->y -= player->dir_y * player->move_speed;
}

void	draw(t_info *info)
{
	int	x;
	int	y;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < info->width)
		{
			info->img.data[y * info->width + x] = info->buf[y][x];
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(info->mlx, info->win, info->img.img, 0, 0);
}

void	init_ray(t_info *info, t_player *player, t_ray *ray, int x)
{
	ray->camera_x = 2 * x / (double)info->width - 1;
	ray->dir_x = player->dir_x + player->plane_x * ray->camera_x;
	ray->dir_y = player->dir_y + player->plane_y * ray->camera_x;
	ray->map_x = (int)player->x;
	ray->map_y = (int)player->y;
	ray->delta_dist_x = fabs(1 / ray->dir_x);
	ray->delta_dist_y = fabs(1 / ray->dir_y);
	ray->hit = 0;
}

void	calculate_step_direction(t_player *player, t_ray *ray)
{
	if (ray->dir_x < 0)
	{
		ray->step_x = -1;
		ray->side_dist_x = (player->x - ray->map_x) * ray->delta_dist_x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist_x = (ray->map_x + 1.0 - player->x) * ray->delta_dist_x;
	}
	if (ray->dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_dist_y = (player->y - ray->map_y) * ray->delta_dist_y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist_y = (ray->map_y + 1.0 - player->y) * ray->delta_dist_y;
	}
}

void	perform_dda(t_ray *ray)
{
	while (ray->hit == 0)
	{
		if (ray->side_dist_x < ray->side_dist_y)
		{
			ray->side_dist_x += ray->delta_dist_x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist_y += ray->delta_dist_y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		if (map[ray->map_x][ray->map_y] > 0)
			ray->hit = 1;
	}
}

void	calculate_wall_distance(t_player *player, t_ray *ray)
{
	if (ray->side == 0)
		ray->perp_wall_dist =\
			(ray->map_x - player->x + (1 - ray->step_x) / 2) / ray->dir_x;
	else
		ray->perp_wall_dist =\
			(ray->map_y - player->y + (1 - ray->step_y) / 2) / ray->dir_y;
	ray->line_height = (int)(HEIGHT / ray->perp_wall_dist);
}

void	calculate_wall_height(t_ray *ray)
{
	ray->draw_start = -ray->line_height / 2 + HEIGHT / 2;
	if (ray->draw_start < 0)
		ray->draw_start = 0;
	ray->draw_end = ray->line_height / 2 + HEIGHT / 2;
	if (ray->draw_end >= HEIGHT)
		ray->draw_end = HEIGHT - 1;
}

void	get_wall_texture(t_player *player, t_ray *ray)
{
	ray->tex_num = map[ray->map_x][ray->map_y] - 1;
	if (ray->side == 0)
		ray->wall_x = player->y + ray->perp_wall_dist * ray->dir_y;
	else
		ray->wall_x = player->x + ray->perp_wall_dist * ray->dir_x;
	ray->wall_x -= floor(ray->wall_x);
	ray->tex_x = (int)(ray->wall_x * (double)TEX_WIDTH);
	if (ray->side == 0 && ray->dir_x > 0)
		ray->tex_x = TEX_WIDTH - ray->tex_x - 1;
	if (ray->side == 1 && ray->dir_y < 0)
		ray->tex_x = TEX_WIDTH - ray->tex_x - 1;
}

void	get_wall_color(t_info *info, t_ray *ray, int x)
{
	int	color;
	int	y;

	ray->step = 1.0 * TEX_HEIGHT / ray->line_height;
	ray->tex_pos = (ray->draw_start - HEIGHT / 2\
					+ ray->line_height / 2) * ray->step;
	y = ray->draw_start;
	while (y < ray->draw_end)
	{
		ray->tex_y = (int)ray->tex_pos & (TEX_HEIGHT - 1);
		ray->tex_pos += ray->step;
		color =\
			info->texture[ray->tex_num][TEX_HEIGHT * ray->tex_y + ray->tex_x];
		if (ray->side == 1)
			color = (color >> 1) & 8355711;
		info->buf[y][x] = color;
		y++;
	}
}

void	raycasting(t_info *info, t_player *player)
{
	int	x;

	x = 0;
	cast_floor_ceiling(info, player, &info->fc);
	while (x < info->width)
	{
		init_ray(info, &info->player, &info->ray, x);
		calculate_step_direction(&info->player, &info->ray);
		perform_dda(&info->ray);
		calculate_wall_distance(&info->player, &info->ray);
		calculate_wall_height(&info->ray);
		get_wall_texture(&info->player, &info->ray);
		get_wall_color(info, &info->ray, x);
		info->z_buffer[x] = info->ray.perp_wall_dist;
		x++;
	}
}

void	sort_sprite(t_sprite_ray *s_ray, t_player *player)
{
	int	i;

	i = 0;
	while (i < NUM_SPRITES)
	{
		s_ray->sprite_order[i] = i;
		s_ray->sprite_distance[i] =\
			((player->x - sprite[i].x) * (player->x - sprite[i].x) +\
				(player->y - sprite[i].y) * (player->y - sprite[i].y));
		i++;
	}
}

void	translate_sprite(t_player *player, t_sprite_ray *s_ray, int i)
{
	s_ray->x = sprite[s_ray->sprite_order[i]].x - player->x;
	s_ray->y = sprite[s_ray->sprite_order[i]].y - player->y;
	s_ray->inversion_detection = 1.0 /\
		(player->plane_x * player->dir_y - player->dir_x * player->plane_y);
	s_ray->transform_x = s_ray->inversion_detection *\
		(player->dir_y * s_ray->x - player->dir_x * s_ray->y);
	s_ray->transform_y = s_ray->inversion_detection *\
		(-player->plane_y * s_ray->x + player->plane_x * s_ray->y);
	s_ray->screen_x = (int)((WIDTH / 2) *\
						(1 + s_ray->transform_x / s_ray->transform_y));
	#define uDiv		1
	#define vDiv		1
	#define vMove		0.0
	s_ray->v_move_screen = (int)(vMove / s_ray->transform_y);
}

void	calculate_sprite_height(t_sprite_ray *s_ray)
{
	s_ray->height = (int)fabs((HEIGHT / s_ray->transform_y) / vDiv);
	s_ray->draw_start_y =\
		-s_ray->height / 2 + HEIGHT / 2 + s_ray->v_move_screen;
	if (s_ray->draw_start_y < 0)
		s_ray->draw_start_y = 0;
	s_ray->draw_end_y = s_ray->height / 2 + HEIGHT / 2 + s_ray->v_move_screen;
	if (s_ray->draw_end_y >= HEIGHT)
		s_ray->draw_end_y = HEIGHT - 1;
}

void	calculate_sprite_width(t_info *info, t_sprite_ray *s_ray)
{
	s_ray->width = (int)fabs((HEIGHT / s_ray->transform_y) / uDiv);
	s_ray->draw_start_x = -s_ray->width / 2 + s_ray->screen_x;
	if (s_ray->draw_start_x < 0)
		s_ray->draw_start_x = 0;
	s_ray->draw_end_x = s_ray->width / 2 + s_ray->screen_x;
	if (s_ray->draw_end_x >= info->width)
		s_ray->draw_end_x = info->width - 1;
}

void	get_sprite_color(t_info *info, t_sprite_ray *s_ray, int i, int stripe)
{
	int	color;
	int	y;
	int	draw;

	s_ray->tex_x =\
		(int)((256 * (stripe - (-s_ray->width / 2 + s_ray->screen_x)) *\
			TEX_WIDTH / s_ray->width) / 256);
	if (s_ray->transform_y > 0 && stripe > 0 &&\
		stripe < WIDTH && s_ray->transform_y < info->z_buffer[stripe])
	{
		y = s_ray->draw_start_y;
		while (y < s_ray->draw_end_y)
		{
			draw = (y - s_ray->v_move_screen) * 256 -\
					HEIGHT * 128 + s_ray->height * 128;
			s_ray->tex_y = ((draw * TEX_HEIGHT) / s_ray->height) / 256;
			color = info->texture[sprite[s_ray->sprite_order[i]].texture]\
									[TEX_WIDTH * s_ray->tex_y + s_ray->tex_x];
			if ((color & 0x00FFFFFF) != 0)
				info->buf[y][stripe] = color;
			y++;
		}
	}
}

void	sprite_raycasting(t_info *info, t_player *player)
{
	int	i;
	int	stripe;

	sort_sprite(&info->s_ray, player);
	i = 0;
	while (i < NUM_SPRITES)
	{
		translate_sprite(player, &info->s_ray, i);
		calculate_sprite_height(&info->s_ray);
		calculate_sprite_width(info, &info->s_ray);
		stripe = info->s_ray.draw_start_x;
		while (stripe < info->s_ray.draw_end_x)
		{
			get_sprite_color(info, &info->s_ray, i, stripe);
			stripe++;
		}
		i++;
	}
}

int	main_loop(t_info *info)
{
	raycasting(info, &info->player);
	sprite_raycasting(info, &info->player);
	draw(info);
	return (0);
}

int	main(int argc, char **argv)
{
	t_info	info;
	int		save;

	save = (argc >= 2 && (ft_strncmp(argv[1], "--save", 6) == 0) ? 1 : 0);
	if (argc < save + 2)
		return (exit_error(&info));
	if (!parse_cub(&info, argv[1 + save]))
		return (exit_error(&info));
	init_player(&info.player);
	if (!init_buffer(&info))
		return (exit_error(&info));
	if (init_texture(&info) == -1)
		return (exit_error(&info));
	load_texture(&info);
	mlx_loop_hook(info.mlx, &main_loop, &info);
	mlx_hook(info.win, X_EVENT_KEY_PRESS, 1 << 0, &key_press, &info);
	mlx_loop(info.mlx);
}
