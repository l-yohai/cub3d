/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/15 13:39:47 by yohlee            #+#    #+#             */
/*   Updated: 2020/07/07 12:29:40 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../mlx/mlx.h"
# include "../libft/libft.h"
# include "get_next_line.h"
# include "key_macos.h"
# include <math.h>
# include <fcntl.h>

# define X_EVENT_KEY_PRESS 2
# define X_EVENT_KEY_EXIT 17
# define PI 3.1415926535897
# define TEX_WIDTH 64
# define TEX_HEIGHT 64

typedef struct				s_img
{
	void					*img;
	int						*data;
	int						size_l;
	int						bpp;
	int						endian;
	int						width;
	int						height;
}							t_img;

typedef struct				s_sprite
{
	double					x;
	double					y;
	double					distance;
}							t_sprite;

typedef struct				s_ray
{
	double					camera_x;
	double					dir_x;
	double					dir_y;
	int						map_x;
	int						map_y;
	double					side_dist_x;
	double					side_dist_y;
	double					delta_dist_x;
	double					delta_dist_y;
	double					perp_wall_dist;
	int						step_x;
	int						step_y;
	int						hit;
	int						side;
	int						line_height;
	int						draw_start;
	int						draw_end;
	int						tex_num;
	double					wall_x;
	double					step;
	double					tex_pos;
	int						tex_x;
	int						tex_y;
}							t_ray;

typedef struct				s_sprite_ray
{
	double					x;
	double					y;
	double					inversion_detection;
	double					transform_x;
	double					transform_y;
	int						screen_x;
	int						v_move_screen;
	int						height;
	int						width;
	int						draw_start_x;
	int						draw_start_y;
	int						draw_end_x;
	int						draw_end_y;
	int						tex_x;
	int						tex_y;
}							t_sprite_ray;

typedef struct				s_player
{
	double					x;
	double					y;
	double					dir_x;
	double					dir_y;
	double					plane_x;
	double					plane_y;
	double					move_speed;
	double					rot_speed;
}							t_player;

typedef struct				s_info
{
	void					*mlx;
	void					*win;
	t_img					img;
	t_player				player;
	t_ray					ray;
	t_sprite_ray			s_ray;
	t_sprite				*sprite;
	int						**buf;
	double					*z_buffer;
	int						**texture;
	int						fd;
	int						width;
	int						height;
	int						map_width;
	int						map_height;
	char					**map;
	char					*north_texture_path;
	char					*south_texture_path;
	char					*west_texture_path;
	char					*east_texture_path;
	char					*sprite_texture_path;
	int						floor_color;
	int						ceiling_color;
	int						dir;
	int						num_sprite;
}							t_info;

/*
**		init.c
*/

void						init_info(t_info *info);
void						init_player(t_player *player);
int							init_buffer(t_info *info);
int							init_texture(t_info *info);
int							init_map(t_info *info, t_list *lst);

/*
**		texture.c
*/

void						cast_floor_ceiling(t_info *info);
void						load_image(t_info *info,\
										int *texture, char *path, t_img *img);
void						load_texture(t_info *info);

/*
**		key_handling.c
*/

void						init_player_direction(t_info *info);
void						move_player(t_info *info,\
										t_player *player, double move_speed);
void						move_player_left_right(t_info *info,\
										t_player *player, double move_speed);
void						rotate_player(t_player *player, double rot_speed);
int							key_press(int key, t_info *info);

/*
**		parse.c
*/

int							get_screen_size(t_info *info, char *line);
int							get_texture(t_info *info,\
										char *line, char *direction);
int							get_color(t_info *info, char *line, int direction);
int							parse_line(t_info *info, char *line);
int							parse_cub(t_info *info, char *path);

/*
**		parse_map.c
*/

int							parse_map(t_info *info, t_list *lst);
int							read_map(t_info *info, char *line);
int							init_direction(t_info *info,\
											int i, int j, int *dir_count);
int							fill_map(t_info *info, t_list *lst);
int							check_valid_map(t_info *info);

/*
**		check_map.c
*/

int							check_map_left(t_info *info, int i, int j);
int							check_map_top(t_info *info);
int							check_map_right(t_info *info, int i, int j);
int							check_map_bottom(t_info *info, int i);
int							check_map_middle(t_info *info, int i, int j);

/*
**		utils.c
*/

int							exit_error(t_info *info);
int							check_init(t_info *info);
void						ft_free_2d(char **str);
int							is_dir(int c);
int							is_map_arg(int c);

/*
**		raycasting.c
*/

void						init_ray(t_info *info,\
										t_player *player, t_ray *ray, int x);
void						calculate_step_direction(t_player *player,\
														t_ray *ray);
void						perform_dda(t_info *info, t_ray *ray);
void						calculate_wall_distance(t_player *player,\
														t_ray *ray);
void						calculate_wall_height(t_info *info, t_ray *ray);

/*
**		draw.c
*/

void						draw(t_info *info);
void						get_wall_texture(t_player *player, t_ray *ray);
void						get_wall_color(t_info *info, t_ray *ray, int x);

/*
**		sprite.c
*/

void						sort_sprite(t_info *info, t_player *player);
void						translate_sprite(t_info *info,\
							t_player *player, t_sprite_ray *s_ray, int i);
void						calculate_sprite_height(t_info *info,\
													t_sprite_ray *s_ray);
void						calculate_sprite_width(t_info *info,\
													t_sprite_ray *s_ray);
void						get_sprite_color(t_info *info,\
											t_sprite_ray *s_ray, int stripe);

/*
**		sprite_utils.c
*/

int							get_max_line_size(t_list *lst);
void						sort(t_info *info);

int							set_pos_sprite(t_info *info, int i, int x, int y);
int							set_sprite(t_info *info);

/*
**		main_loop.c
*/

void						raycasting(t_info *info);
void						sprite_raycasting(t_info *info, t_player *player);
int							main_loop(t_info *info);

/*
**		bmp.c
*/

void						set_int_in_char(unsigned char *start, int value);
void						write_bmp_header(t_info *info, int fd, int size);
void						write_data(t_info *info, int fd);
int							save_bmp(t_info *info);

/*
**		exit.c
*/

int							ft_exit(int ret);
void						ft_free(void *s);
int							free_cub(t_info *info);

/*
**		main.c
*/

int							set_info(t_info *info, char *path);
int							process_option(t_info *info, int option);

#endif
