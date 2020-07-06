/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/01 11:15:43 by yohlee            #+#    #+#             */
/*   Updated: 2020/07/05 23:35:44 by yohlee           ###   ########.fr       */
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
	info->width = 0;
	info->height = 0;
	info->map_width = 0;
	info->map_height = 0;
	info->map = NULL;
	info->north_texture_path = NULL;
	info->south_texture_path = NULL;
	info->west_texture_path = NULL;
	info->east_texture_path = NULL;
	info->sprite_texture_path = NULL;
	info->floor_color = 0;
	info->ceiling_color = 0;
}

// void	init_player(t_player *player, int i, int j, int direction)
// {
// 	player->x = j;
// 	player->y = i;
// 	if (direction == 'N')
// 	{
// 		player->dir_x = 0.0;
// 		player->dir_y = 1.0;
// 	}
// 	else if (direction == 'E')
// 	{
// 		player->dir_x = 1.0;
// 		player->dir_y = 0.0;
// 	}
// 	else if (direction == 'S')
// 	{
// 		player->dir_x = 0.0;
// 		player->dir_y = -1.0;
// 	}
// 	else if (direction == 'W')
// 	{
// 		player->dir_x = -1.0;
// 		player->dir_y = 0.0;
// 	}
// 	player->plane_x = 0.0;
// 	player->plane_y = 0.66;
// 	player->move_speed = 0.05;
// 	player->rot_speed = 0.05;
// }

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

int		init_buffer(t_info *info)
{
	int	i;
	int	j;

	i = 0;
	if (!(info->buf = ft_calloc(HEIGHT, sizeof(int **))))
		return (exit_error(info));
	while (i < HEIGHT)
	{
		j = 0;
			if (!(info->buf[i] = ft_calloc(WIDTH, sizeof(int *))))
				return (exit_error(info));
		i++;
	}
	if (!(info->z_buffer = ft_calloc(WIDTH, sizeof(double *))))
		return (exit_error(info));
	return (1);
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

int	init_map(t_info *info, t_list *lst)
{
	int	i;
	int	len;

	info->map_height = ft_lstsize(lst);
	if (!(info->map = ft_calloc(info->map_height, sizeof(char **))))
		return (exit_error(info));
	i = 0;
	len = 0;
	while (lst)
	{
		len = ft_strlen(lst->content);
		if (!(info->map[i] = ft_calloc(len, sizeof(char *))))
			return (exit_error(info));
		if (info->map_width < len)
			info->map_width = len;
		i++;
		lst = lst->next;
	}
	return (1);
}
