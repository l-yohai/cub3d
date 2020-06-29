#include <stdio.h>
#include <stdlib.h>
#include "mlx/mlx.h"
typedef struct s_img
{
	void		*img;
	int			*data;
	int			width;
	int			height;


	int			size_l;
	int			bpp;
	int			endian;
}				t_img;

typedef struct	s_info
{
	void	*mlx;
	void	*win;
	t_img	img;
	// int		texture[8][texHeight * texWidth];
	// int		**texture;
	int		*texture;
}				t_info;

int		get_color(t_img *idata)
{
	int		result;
	int		max;
	int		i;

	result = 0;
	i = 0;
	max = idata->bpp / 8;
	while (i < max)
	{
		result *= 256;
		if (idata->endian == 0)
			result += (int)(idata->data[max - 1 - i]);
		else
			result += (int)(idata->data[i]);
		i++;
	}
	idata->data += max;
	return (result);
}

void	load_texture(t_info *info)
{
	t_img	img;
	int		*res;

	img.img = mlx_xpm_file_to_image(info->mlx, "textures/wood.xpm", &img.width, &img.height);
	img.data = (int *)mlx_get_data_addr(img.img, &img.bpp, &img.size_l, &info->img.endian);

	res = (int *)malloc(sizeof(int) * (img.width * img.height));
	for (int y = 0; y < img.height; y++)
	{
		for (int x = 0; x < img.width; x++)
		{
			res[img.width * y + x] = get_color(&img);
		}
	}
	info->texture = res;
}

int main()
{
	t_info	info;

	info.mlx = mlx_init();
	info.win = mlx_new_window(info.mlx, 640, 480, "my_mlx");

	// if (!(info.texture = (int **)malloc(sizeof(int *) * 8)))
	// 	return (-1);
	// for (int i = 0; i < 8; i++)
	// {
	// 	if (!(info.texture[i] = (int *)malloc(sizeof(int) * (64 * 64))))
	// 		return (-1);
	// }
	// for (int i = 0; i < 8; i++)
	// {
	// 	for (int j = 0; j < 64 * 64; j++)
	// 	{
	// 		info.texture[i][j] = 0;
	// 	}
	// }

	// load_texture(&info);

	load_texture(&info);


	// info.img.img = mlx_xpm_file_to_image(info.mlx, "textures/eagle.xpm", &info.img.width, &info.img.height);
	// info.img.data = (int *)mlx_get_data_addr(info.img.img, &info.img.bpp, &info.img.size_l, &info.img.endian);
	// mlx_destroy_image(info.mlx, info.img.img);

	// info.img.img = mlx_xpm_file_to_image(info.mlx, "textures/barrel.xpm", &info.img.width, &info.img.height);
	// info.img.data = (int *)mlx_get_data_addr(info.img.img, &info.img.bpp, &info.img.size_l, &info.img.endian);
	// mlx_destroy_image(info.mlx, info.img.img);

	// mlx_put_image_to_window(info.mlx, info.win, info.img.img, 50, 50);
	mlx_loop(info.mlx);
	return (0);
}