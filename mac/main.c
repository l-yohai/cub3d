/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/01 13:13:22 by yohlee            #+#    #+#             */
/*   Updated: 2020/07/01 17:03:25 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	worldMap[MAP_WIDTH][MAP_HEIGHT] =
						{
							{4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,7,7,7,7,7,7,7,7},
							{4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
							{4,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
							{4,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
							{4,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
							{4,0,4,0,0,0,0,5,5,5,5,5,5,5,5,5,7,7,0,7,7,7,7,7},
							{4,0,5,0,0,0,0,5,0,5,0,5,0,5,0,5,7,0,0,0,7,7,7,1},
							{4,0,6,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
							{4,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,1},
							{4,0,8,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
							{4,0,0,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,7,7,7,1},
							{4,0,0,0,0,0,0,5,5,5,5,0,5,5,5,5,7,7,7,7,7,7,7,1},
							{6,6,6,6,6,6,6,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
							{8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
							{6,6,6,6,6,6,0,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
							{4,4,4,4,4,4,0,4,4,4,6,0,6,2,2,2,2,2,2,2,3,3,3,3},
							{4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
							{4,0,0,0,0,0,0,0,0,0,0,0,6,2,0,0,5,0,0,2,0,0,0,2},
							{4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
							{4,0,6,0,6,0,0,0,0,4,6,0,0,0,0,0,5,0,0,0,0,0,0,2},
							{4,0,0,5,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
							{4,0,6,0,6,0,0,0,0,4,6,0,6,2,0,0,5,0,0,2,0,0,0,2},
							{4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
							{4,4,4,4,4,4,4,4,4,4,1,1,1,2,2,2,2,2,2,3,3,3,3,3}
						};

void	draw(t_info *info)
{
	int	x;
	int	y;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			info->img.data[y * WIDTH + x] = info->buffer[y][x];
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(info->mlx, info->win, info->img.img, 0, 0);
}

void	init_ray(t_info *info, t_ray *ray, int x)
{
	ray->camera_x = 2 * x / (double)WIDTH - 1;
	ray->dir.x = info->player.dir.x + info->player.plane.x * ray->camera_x;
	ray->dir.y = info->player.dir.y + info->player.plane.y * ray->camera_x;
	ray->map_x = (int)info->player.pos.x;
	ray->map_y = (int)info->player.pos.y;
	ray->delta_dist.x = fabs(1 / ray->dir.x);
	ray->delta_dist.y = fabs(1 / ray->dir.y);
	ray->hit = 0;
}

void	calculate_step_direction(t_info *info, t_ray *ray)
{
	if (ray->dir.x < 0)
	{
		ray->step_x = -1;
		ray->side_dist.x =\
			(info->player.pos.x - ray->map_x) * ray->delta_dist.x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist.x =\
			(ray->map_x + 1.0 - info->player.pos.x) * ray->delta_dist.x;
	}
	if (ray->dir.y < 0)
	{
		ray->step_y = -1;
		ray->side_dist.y =\
			(info->player.pos.y - ray->map_y) * ray->delta_dist.y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist.y =\
			(ray->map_y + 1.0 - info->player.pos.y) * ray->delta_dist.y;
	}
}

void	perform_dda(t_ray *ray)
{
	while (ray->hit == 0)
	{
		if (ray->side_dist.x < ray->side_dist.y)
		{
			ray->side_dist.x += ray->delta_dist.x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist.y += ray->delta_dist.y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		if (worldMap[ray->map_x][ray->map_y] > 0)
			ray->hit = 1;
	}
}

void	calculate_wall_distance(t_info *info, t_ray *ray)
{
	if (ray->side == 0)
		ray->perp_wall_dist =\
			(ray->map_x - info->player.pos.x +\
				(1 - ray->step_x) / 2) / ray->dir.x;
	else
		ray->perp_wall_dist =\
			(ray->map_y - info->player.pos.y +\
				(1 - ray->step_y) / 2) / ray->dir.y;
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

void	calculate_wall_surface(t_info *info, t_ray *ray)
{
	if (ray->side == 0)
		ray->wall_x = info->player.pos.y + ray->perp_wall_dist * ray->dir.y;
	else
		ray->wall_x = info->player.pos.x + ray->perp_wall_dist * ray->dir.x;
	ray->wall_x -= floor(ray->wall_x);
}

void	calculate_texture_coordinate(t_info *info, t_ray *ray)
{
	info->tex_x = (int)(ray->wall_x * (double)TEX_WIDTH);
	if (ray->side == 0 && ray->dir.x > 0)
		info->tex_x = TEX_WIDTH - info->tex_x - 1;
	if (ray->side == 1 && ray->dir.y < 0)
		info->tex_x = TEX_WIDTH - info->tex_x - 1;
}

void	calculate_texture_color(t_info *info, t_ray *ray, int x)
{
	double	step;
	double	pos;
	int		color;
	int		y;

	step = 1.0 * TEX_HEIGHT / ray->line_height;
	pos = (ray->draw_start - HEIGHT / 2 + ray->line_height / 2) * step;
	y = ray->draw_start;
	while (y < ray->draw_end)
	{
		info->tex_y = (int)pos & (TEX_HEIGHT - 1);
		pos += step;
		color = info->texture[info->tex_num][TEX_HEIGHT *\
												info->tex_y + info->tex_x];
		if (ray->side == 1)
			color = (color >> 1) & 8355711;
		info->buffer[y][x] = color;
		y++;
	}
}

int	main_loop(t_info *info)
{
	int	x;

	x = 0;
	while (x < WIDTH)
	{
		init_ray(info, &info->ray, x);
		calculate_step_direction(info, &info->ray);
		perform_dda(&info->ray);
		calculate_wall_distance(info, &info->ray);
		calculate_wall_height(&info->ray);
		calculate_wall_surface(info, &info->ray);
		calculate_texture_coordinate(info, &info->ray);
		calculate_texture_color(info, &info->ray, x);
		draw(info);
		x++;
	}
	return (0);
}

void	key_press_up(t_info *info)
{
	if (!worldMap[(int)(info->player.pos.x +\
					info->player.dir.x * info->player.move_speed)]\
			[(int)(info->player.pos.y)])
		info->player.pos.x += info->player.dir.x * info->player.move_speed;
	if (!worldMap[(int)(info->player.pos.x)]\
			[(int)(info->player.pos.y +\
					info->player.dir.y * info->player.move_speed)])
		info->player.pos.y += info->player.dir.y * info->player.move_speed;
}

void	key_press_down(t_info *info)
{
	if (worldMap[(int)(info->player.pos.x -\
					info->player.dir.x * info->player.move_speed)]\
			[(int)(info->player.pos.y)])
		info->player.pos.x -= info->player.dir.x * info->player.move_speed;
	if (worldMap[(int)(info->player.pos.x)]\
			[(int)(info->player.pos.y -\
					info->player.dir.y * info->player.move_speed)])
		info->player.pos.y -= info->player.dir.y * info->player.move_speed;
}

void	key_press_right(t_info *info)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = info->player.dir.x;
	info->player.dir.x = info->player.dir.x * cos(-info->player.rot_speed) -\
					info->player.dir.y * sin(-info->player.rot_speed);
	info->player.dir.y = old_dir_x * sin(-info->player.rot_speed) +\
					info->player.dir.y * cos(-info->player.rot_speed);
	old_plane_x = info->player.plane.x;
	info->player.plane.x = info->player.plane.x * cos(-info->player.rot_speed)\
						- info->player.plane.y * sin(-info->player.rot_speed);
	info->player.plane.y = old_plane_x * sin(-info->player.rot_speed) +\
					info->player.plane.y * cos(-info->player.rot_speed);
}

void	key_press_left(t_info *info)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = info->player.dir.x;
	info->player.dir.x = info->player.dir.x * cos(info->player.rot_speed) -\
					info->player.dir.y * sin(info->player.rot_speed);
	info->player.dir.y = old_dir_x * sin(info->player.rot_speed) +\
					info->player.dir.y * cos(info->player.rot_speed);
	old_plane_x = info->player.plane.x;
	info->player.plane.x = info->player.plane.x * cos(info->player.rot_speed) -\
					info->player.plane.y * sin(info->player.rot_speed);
	info->player.plane.y = old_plane_x * sin(info->player.rot_speed) +\
					info->player.plane.y * cos(info->player.rot_speed);
}

int		key_press(int key, t_info *info)
{
	if (key == K_W || key == K_AR_U)
		key_press_up(info);
	if (key == K_S || key == K_AR_D)
		key_press_down(info);
	if (key == K_A || key == K_AR_L)
		key_press_left(info);
	if (key == K_D || key == K_AR_R)
		key_press_right(info);
	if (key == K_ESC)
		exit(0);
	return (0);
}

void	init_player(t_player *player)
{
	player->pos.x = 12;
	player->pos.y = 5;
	player->dir.x = -1;
	player->dir.y = 0;
	player->plane.x = 0;
	player->plane.y = 0.66;
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
			info->buffer[i][j] = 0;
			j++;
		}
		i++;
	}
}

int	*load_image(t_info *info, char *path, t_img *img)
{
	int		*res;
	int		x;
	int		y;

	img->img = mlx_xpm_file_to_image(info->mlx,\
						path, &img->width, &img->height);
	img->data = (int *)mlx_get_data_addr(img->img,\
						&img->bpp, &img->size_l, &img->endian);
	res = (int *)malloc(sizeof(int) * (img->width * img->height));
	y = 0;
	while (y < img->height)
	{
		x = 0;
		while (x < img->width)
		{
			res[img->width * y + x] = img->data[img->width * y + x];
			x++;
		}
		y++;
	}
	mlx_destroy_image(info->mlx, img->img);
	return (res);
}

void	load_texture(t_info *info)
{
	t_img	img;

	info->texture[0] = load_image(info, "textures/eagle.xpm", &img);
	info->texture[1] = load_image(info, "textures/redbrick.xpm", &img);
	info->texture[2] = load_image(info, "textures/purplestone.xpm", &img);
	info->texture[3] = load_image(info, "textures/greystone.xpm", &img);
	info->texture[4] = load_image(info, "textures/bluestone.xpm", &img);
	info->texture[5] = load_image(info, "textures/mossy.xpm", &img);
	info->texture[6] = load_image(info, "textures/wood.xpm", &img);
	info->texture[7] = load_image(info, "textures/colorstone.xpm", &img);
}

int	init_texture(t_info *info)
{
	int	i;
	int	j;

	if (!(info->texture = (int **)malloc(sizeof(int *) * 8)))
		return (-1);
	i = 0;
	while (i < 8)
	{
		if (!(info->texture[i] =\
				(int *)malloc(sizeof(int) * (TEX_HEIGHT * TEX_WIDTH))))
			return (-1);
		i++;
	}
	i = 0;
	while (i < 8)
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

int	main(void)
{
	t_info	info;

	info.mlx = mlx_init();
	info.win = mlx_new_window(info.mlx, 640, 480, "mlx");
	init_player(&info.player);
	init_buffer(&info);
	init_texture(&info);
	load_texture(&info);
	info.img.img = mlx_new_image(info.mlx, WIDTH, HEIGHT);
	info.img.data = (int *)mlx_get_data_addr(info.img.img,\
						&info.img.bpp, &info.img.size_l, &info.img.endian);
	mlx_loop_hook(info.mlx, &main_loop, &info);
	mlx_hook(info.win, X_EVENT_KEY_PRESS, 0, &key_press, &info);
	mlx_loop(info.mlx);
}
