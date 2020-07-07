/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/01 19:24:10 by yohlee            #+#    #+#             */
/*   Updated: 2020/07/07 10:57:33 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	cast_floor_ceiling(t_info *info)
{
	int	y;
	int	x;

	y = 0;
	while (y < info->height)
	{
		x = 0;
		while (x < info->width)
		{
			if (y > info->height / 2)
				info->buf[y][x] = info->floor_color;
			else
				info->buf[y][x] = info->ceiling_color;
			x++;
		}
		y++;
	}
}

void	load_image(t_info *info, int *texture, char *path, t_img *img)
{
	int		x;
	int		y;

	img->img = mlx_xpm_file_to_image(\
					info->mlx, path, &img->width, &img->height);
	img->data = (int *)mlx_get_data_addr(\
					img->img, &img->bpp, &img->size_l, &img->endian);
	y = 0;
	while (y < img->height)
	{
		x = 0;
		while (x < img->width)
		{
			texture[img->width * y + x] =\
					img->data[img->width * y + x];
			x++;
		}
		y++;
	}
	mlx_destroy_image(info->mlx, img->img);
}

void	load_texture(t_info *info)
{
	t_img	img;

	load_image(info, info->texture[3], info->north_texture_path, &img);
	load_image(info, info->texture[2], info->south_texture_path, &img);
	load_image(info, info->texture[0], info->west_texture_path, &img);
	load_image(info, info->texture[1], info->east_texture_path, &img);
	load_image(info, info->texture[4], info->sprite_texture_path, &img);
	ft_free(info->north_texture_path);
	ft_free(info->south_texture_path);
	ft_free(info->west_texture_path);
	ft_free(info->east_texture_path);
	ft_free(info->sprite_texture_path);
}
