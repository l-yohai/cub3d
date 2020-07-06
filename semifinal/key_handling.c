/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_handling.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/05 07:42:15 by yohlee            #+#    #+#             */
/*   Updated: 2020/07/05 23:10:29 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	key_press_right(t_player *player)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = player->dir_x;
	player->dir_x = player->dir_x * cos(-player->rot_speed) -\
					player->dir_y * sin(-player->rot_speed);
	player->dir_y = old_dir_x * sin(-player->rot_speed) +\
					player->dir_y * cos(-player->rot_speed);
	old_plane_x = player->plane_x;
	player->plane_x = player->plane_x * cos(-player->rot_speed) -\
					player->plane_y * sin(-player->rot_speed);
	player->plane_y = old_plane_x * sin(-player->rot_speed) +\
					player->plane_y * cos(-player->rot_speed);
}

void	key_press_left(t_player *player)
{
	double old_dir_x;
	double old_plane_x;

	old_dir_x = player->dir_x;
	player->dir_x = player->dir_x * cos(player->rot_speed) -\
					player->dir_y * sin(player->rot_speed);
	player->dir_y = old_dir_x * sin(player->rot_speed) +\
					player->dir_y * cos(player->rot_speed);
	old_plane_x = player->plane_x;
	player->plane_x = player->plane_x * cos(player->rot_speed) -\
					player->plane_y * sin(player->rot_speed);
	player->plane_y = old_plane_x * sin(player->rot_speed) +\
					player->plane_y * cos(player->rot_speed);
}

int	key_press(int key, t_info *info)
{
	if (key == K_W)
		key_press_up(&info->player);
	if (key == K_S)
		key_press_down(&info->player);
	if (key == K_D)
		key_press_right(&info->player);
	if (key == K_A)
		key_press_left(&info->player);
	if (key == K_ESC)
		exit(0);
	return (0);
}
