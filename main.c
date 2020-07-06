/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/01 17:09:52 by yohlee            #+#    #+#             */
/*   Updated: 2020/07/06 10:58:46 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	set_info(t_info *info, char *path)
{
	init_player(&info->player);
	if (!parse_cub(info, path))
		return (exit_error(info));
	init_player_direction(info);
	info->mlx = mlx_init();
	if (!init_buffer(info))
		return (exit_error(info));
	if (!init_texture(info))
		return (exit_error(info));
	load_texture(info);
	info->img.img = mlx_new_image(info->mlx, info->width, info->height);
	info->img.data = (int *)mlx_get_data_addr(\
		info->img.img, &info->img.bpp, &info->img.size_l, &info->img.endian);
	return (1);
}

int	exit_free(t_info *info)
{
	if (info->map != NULL)
		ft_free_2d(info->map);
	if (info->buf != NULL)
		free(info->buf);
		// ft_free_2d(info->buf);
	if (info->texture != NULL)
		free(info->texture);
		// ft_free_2d(info->texture);
	if (info->z_buffer != NULL)
		free(info->z_buffer);
	if (info->sprite != NULL)
		free(info->sprite);
	if (info->img.img != NULL)
		mlx_destroy_image(info->mlx, info->img.img);
	if (info->mlx != NULL && info->win != NULL)
		mlx_destroy_window(info->mlx, info->win);
	exit(0);
	return (0);
}

int	main(int argc, char **argv)
{
	t_info	info;
	int		save;

	save = (argc >= 2 && (ft_strncmp(argv[1], "--save", 6) == 0)) ? 1 : 0;
	if (argc < save + 2)
		return (exit_error(&info));
	if (!set_info(&info, argv[save + 1]))
		return (exit_error(&info));
	if (save == 1)
	{
		raycasting(&info);
		sprite_raycasting(&info, &info.player);
		if (!save_bmp(&info))
			return (exit_error(&info));
		return (exit_free(&info));
	}
	else
	{
		info.win = mlx_new_window(info.mlx, info.width, info.height, "yohlee");
		mlx_loop_hook(info.mlx, &main_loop, &info);
		mlx_hook(info.win, X_EVENT_KEY_PRESS, 1 << 0, &key_press, &info);
		mlx_loop(info.mlx);
	}
	return (0);
}
