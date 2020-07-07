/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_handling.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/05 07:42:15 by yohlee            #+#    #+#             */
/*   Updated: 2020/07/07 11:17:06 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_player_direction(t_info *info)
{
	int	rot;
	int	direction;

	rot = 0;
	direction = info->dir;
	if (direction == 'N')
		rot = 270;
	else if (direction == 'S')
		rot = 90;
	else if (direction == 'E')
		rot = 0;
	else if (direction == 'W')
		rot = 180;
	rotate_player(&info->player, rot * (PI / 180));
}

void	move_player(t_info *info, t_player *player, double move_speed)
{
	if (info->map[(int)player->y]\
					[(int)(player->x + player->dir_x * move_speed)] != '1')
		player->x += player->dir_x * move_speed;
	if (info->map[(int)(player->y + player->dir_y * move_speed)]\
					[(int)player->x] != '1')
		player->y += player->dir_y * move_speed;
}

void	move_player_left_right(t_info *info,\
								t_player *player, double move_speed)
{
	if (info->map[(int)player->y]\
					[(int)(player->x + player->dir_y * move_speed)] != '1')
		player->x += player->dir_y * move_speed;
	if (info->map[(int)(player->y + -player->dir_x * move_speed)]\
					[(int)player->x] != '1')
		player->y += -player->dir_x * move_speed;
}

void	rotate_player(t_player *player, double rot_speed)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = player->dir_x;
	player->dir_x = player->dir_x * cos(rot_speed) -\
					player->dir_y * sin(rot_speed);
	player->dir_y = old_dir_x * sin(rot_speed) +\
					player->dir_y * cos(rot_speed);
	old_plane_x = player->plane_x;
	player->plane_x = player->plane_x * cos(rot_speed) -\
					player->plane_y * sin(rot_speed);
	player->plane_y = old_plane_x * sin(rot_speed) +\
					player->plane_y * cos(rot_speed);
}

int		key_press(int key, t_info *info)
{
	if (key == K_W || key == K_AR_U)
		move_player(info, &info->player, info->player.move_speed);
	if (key == K_S || key == K_AR_D)
		move_player(info, &info->player, -info->player.move_speed);
	if (key == K_A)
		move_player_left_right(info, &info->player, -info->player.move_speed);
	if (key == K_D)
		move_player_left_right(info, &info->player, info->player.move_speed);
	if (key == K_AR_L)
		rotate_player(&info->player, info->player.rot_speed);
	if (key == K_AR_R)
		rotate_player(&info->player, -info->player.rot_speed);
	if (key == K_ESC)
		ft_exit(0);
	return (0);
}
