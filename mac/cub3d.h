/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/15 13:39:47 by yohlee            #+#    #+#             */
/*   Updated: 2020/07/01 22:03:04 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "mlx/mlx.h"
# include "libft/libft.h"
# include "key_macos.h"
# include <math.h>

#define X_EVENT_KEY_PRESS	2
#define X_EVENT_KEY_EXIT	17

#define TEX_WIDTH	64
#define TEX_HEIGHT	64
#define MAP_WIDTH	24
#define MAP_HEIGHT	24
#define WIDTH		1920
#define HEIGHT		1000
#define NUM_SPRITES	19


typedef struct		s_img
{	
	void			*img;
	int				*data;
	int				size_l;
	int				bpp;
	int				endian;
	int				width;
	int				height;
}					t_img;

typedef struct		s_sprite
{	
	double			x;
	double			y;
	int				texture;
}					t_sprite;

typedef struct		s_floor_ceiling
{	
	float			row_distance;
	float			floor_x;
	float			floor_y;
	int				tex_x;
	int				tex_y;
	int				checker_board_pattern;
	int				floor_texture;
	int				ceiling_texture;
}					t_floor_ceiling;

typedef struct		s_ray
{	
	double			camera_x;
	double			dir_x;
	double			dir_y;
	int				map_x;
	int				map_y;
	double			side_dist_x;
	double			side_dist_y;
	double			delta_dist_x;
	double			delta_dist_y;
	double			perp_wall_dist;
	int				step_x;
	int				step_y;
	int				hit;
	int				side;
	int				line_height;
	int				draw_start;
	int				draw_end;
	int				tex_num;
	double			wall_x;
	double			step;
	double			tex_pos;
	int				tex_x;
	int				tex_y;
}					t_ray;

typedef struct		s_sprite_ray
{
	int				sprite_order[NUM_SPRITES];
	double			sprite_distance[NUM_SPRITES];
	double			x;
	double			y;
	double			inversion_detection;
	double			transform_x;
	double			transform_y;
	int				screen_x;
	int				v_move_screen;
	int				height;
	int				width;
	int				draw_start_x;
	int				draw_start_y;
	int				draw_end_x;
	int				draw_end_y;
	int				tex_x;
	int				tex_y;
}					t_sprite_ray;

typedef struct		s_player
{	
	double			x;
	double			y;
	double			dir_x;
	double			dir_y;
	double			plane_x;
	double			plane_y;
	double			move_speed;
	double			rot_speed;
}					t_player;

typedef struct		s_info
{	
	void			*mlx;
	void			*win;

	t_img			img;
	t_player		player;
	t_floor_ceiling	fc;
	t_ray			ray;
	t_sprite_ray	s_ray;

	int				map[MAP_WIDTH][MAP_HEIGHT];
	int				buf[HEIGHT][WIDTH];
	double			z_buffer[WIDTH];
	
	int				**texture;
	
	int				sprite_order[NUM_SPRITES];
	double			sprite_distance[NUM_SPRITES];
}					t_info;

/*
**				init.c
*/

void	init_info(t_info *info);
void	init_player(t_player *player);
void	init_buffer(t_info *info);
int		init_texture(t_info *info);

/*
**				texture.c
*/

void	load_texture(t_info *info);

/*
**				key_handling.c
*/

int	key_press(int key, t_info *info);

/*
**				floor_ceiling.c
*/

void	cast_floor_ceiling(t_info *info, t_player *player, t_floor_ceiling *fc);


#endif