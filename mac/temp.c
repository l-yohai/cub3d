/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/01 10:36:20 by yohlee            #+#    #+#             */
/*   Updated: 2020/07/01 13:08:00 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#include <string.h>

void	draw(t_info *info)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			info->img.data[y * width + x] = info->buffer[y][x];
		}
	}
	mlx_put_image_to_window(info->mlx, info->win, info->img.img, 0, 0);
}

void	floor_casting(t_info *info, t_ray *ray)
{
	int	x;
	int	y;

	y = height / 2 + 1;
	while (++y < height)
	{
		ray->dir0.x = info->dir.x - info->plane.x;
		ray->dir0.y = info->dir.y - info->plane.y;
		ray->dir1.x = info->dir.x + info->plane.x;
		ray->dir1.y = info->dir.y + info->plane.y;
		ray->pos_y = y - height / 2;
		ray->pos_z = height * 0.5;
		ray->row_distance = ray->pos_z / ray->pos_y;
		ray->floor_step.x = ray->row_distance * (ray->dir1.x - ray->dir0.x) / width;
		ray->floor_step.y = ray->row_distance * (ray->dir1.y - ray->dir0.y) / width;
		ray->floor.x = info->pos.x + ray->row_distance * ray->dir0.x;
		ray->floor.y = info->pos.y + ray->row_distance * ray->dir0.y;
		x = 0;
		while (++x < width)
		{
			ray->cell_x = (int)ray->floor.x;
			ray->cell_y = (int)ray->floor.y;
			ray->tex_x = (int)(texWidth * (ray->floor.x - ray->cell_x)) & (texWidth - 1);
			ray->tex_y = (int)(texHeight * (ray->floor.y - ray->cell_y)) & (texHeight - 1);
			ray->floor.x += ray->floor_step.x;
			ray->floor.y += ray->floor_step.y;

			int floorTexture = 3;
			int ceilingTexture = 6;
			int color;
			color = info->texture[floorTexture][texWidth * ray->tex_y + ray->tex_x];
			color = (color >> 1) & 8355711; // make a bit darker
			info->buffer[y][x] = color;
			color = info->texture[ceilingTexture][texWidth * ray->tex_y + ray->tex_x];
			color = (color >> 1) & 8355711; // make a bit darker
			info->buffer[height - y - 1][x] = color;
		}
	}
}

void	calc(t_info *info)
{
	// WALL CASTING
	for(int x = 0; x < width; x++)
	{
		//calculate ray position and direction
		double cameraX = 2 * x / (double)width - 1; //x-coordinate in camera space
		double rayDirX = info->dir.x + info->plane.x * cameraX;
		double rayDirY = info->dir.y + info->plane.y * cameraX;
		//which box of the map we're in
		int map_x = (int)info->pos.x;
		int map_y = (int)info->pos.y;
		//length of ray from current position to next x or y-side
		double sideDistX;
		double sideDistY;
		//length of ray from one x or y-side to next x or y-side
		double deltaDistX = fabs(1 / rayDirX);
		double deltaDistY = fabs(1 / rayDirY);
		double perpWallDist;
		//what direction to step in x or y-direction (either +1 or -1)
		int stepX;
		int stepY;
		int hit = 0; //was there a wall hit?
		int side; //was a NS or a EW wall hit?
		//calculate step and initial sideDist
		if(rayDirX < 0)
		{
			stepX = -1;
			sideDistX = (info->pos.x - map_x) * deltaDistX;
		}
		else
		{
			stepX = 1;
			sideDistX = (map_x + 1.0 - info->pos.x) * deltaDistX;
		}
		if(rayDirY < 0)
		{
			stepY = -1;
			sideDistY = (info->pos.y - map_y) * deltaDistY;
		}
		else
		{
			stepY = 1;
			sideDistY = (map_y + 1.0 - info->pos.y) * deltaDistY;
		}
		//perform DDA
		while (hit == 0)
		{
			//jump to next map square, OR in x-direction, OR in y-direction
			if(sideDistX < sideDistY)
			{
				sideDistX += deltaDistX;
				map_x += stepX;
				side = 0;
			}
			else
			{
				sideDistY += deltaDistY;
				map_y += stepY;
				side = 1;
			}
			//Check if ray has hit a wall
			if (info->map[map_x][map_y] > 0)
				hit = 1;
		}
		//Calculate distance of perpendicular ray (Euclidean distance will give fisheye effect!)
		if (side == 0)
			perpWallDist = (map_x - info->pos.x + (1 - stepX) / 2) / rayDirX;
		else
			perpWallDist = (map_y - info->pos.y + (1 - stepY) / 2) / rayDirY;
		//Calculate height of line to draw on screen
		int lineHeight = (int)(height / perpWallDist);
		//calculate lowest and highest pixel to fill in current stripe
		int drawStart = -lineHeight / 2 + height / 2;
		if(drawStart < 0) drawStart = 0;
		int drawEnd = lineHeight / 2 + height / 2;
		if(drawEnd >= height) drawEnd = height - 1;
		//texturing calculations
		int texNum = info->map[map_x][map_y] - 1; //1 subtracted from it so that texture 0 can be used!
		//calculate value of wallX
		double wallX; //where exactly the wall was hit
		if (side == 0) wallX = info->pos.y + perpWallDist * rayDirY;
		else           wallX = info->pos.x + perpWallDist * rayDirX;
		wallX -= floor((wallX));
		//x coordinate on the texture
		int texX = (int)(wallX * (double)texWidth);
		if(side == 0 && rayDirX > 0) texX = texWidth - texX - 1;
		if(side == 1 && rayDirY < 0) texX = texWidth - texX - 1;
		// TODO: an integer-only bresenham or DDA like algorithm could make the texture coordinate stepping faster
		// How much to increase the texture coordinate per screen pixel
		double step = 1.0 * texHeight / lineHeight;
		// Starting texture coordinate
		double texPos = (drawStart - height / 2 + lineHeight / 2) * step;
		for(int y = drawStart; y < drawEnd; y++)
		{
			// Cast the texture coordinate to integer, and mask with (texHeight - 1) in case of overflow
			int texY = (int)texPos & (texHeight - 1);
			texPos += step;
			int color = info->texture[texNum][texHeight * texY + texX];
			//make color darker for y-sides: R, G and B byte each divided through two with a "shift" and an "and"
			if(side == 1) color = (color >> 1) & 8355711;
			info->buffer[y][x] = color;
		}

		//SET THE ZBUFFER FOR THE SPRITE CASTING
		info->z_buffer[x] = perpWallDist; //perpendicular distance is used
	}

	//SPRITE CASTING
	//sort sprites from far to close
	for(int i = 0; i < numSprites; i++)
	{
		spriteOrder[i] = i;
		spriteDistance[i] = ((info->pos.x - info->sprite[i].x) * (info->pos.x - info->sprite[i].x) + (info->pos.y - info->sprite[i].y) * (info->pos.y - info->sprite[i].y)); //sqrt not taken, unneeded
	}
	// sortSprites(spriteOrder, spriteDistance, numSprites);

	//after sorting the sprites, do the projection and draw them
	for(int i = 0; i < numSprites; i++)
	{
		//translate sprite position to relative to camera
		double spriteX = info->sprite[spriteOrder[i]].x - info->pos.x;
		double spriteY = info->sprite[spriteOrder[i]].y - info->pos.y;

		//transform sprite with the inverse camera matrix
		// [ planeX   dirX ] -1                                       [ dirY      -dirX ]
		// [               ]       =  1/(planeX*dirY-dirX*planeY) *   [                 ]
		// [ planeY   dirY ]                                          [ -planeY  planeX ]

		double invDet = 1.0 / (info->plane.x * info->dir.y - info->dir.x * info->plane.y); //required for correct matrix multiplication

		double transformX = invDet * (info->dir.y * spriteX - info->dir.x * spriteY);
		double transformY = invDet * (-info->plane.y * spriteX + info->plane.x * spriteY); //this is actually the depth inside the screen, that what Z is in 3D, the distance of sprite to player, matching sqrt(spriteDistance[i])

		int spriteScreenX = (int)((width / 2) * (1 + transformX / transformY));

		//parameters for scaling and moving the sprites
		#define uDiv 1
		#define vDiv 1
		#define vMove 0.0
		int vMoveScreen = (int)(vMove / transformY);

		//calculate height of the sprite on screen
		int spriteHeight = (int)fabs((height / transformY) / vDiv); //using "transformY" instead of the real distance prevents fisheye
		//calculate lowest and highest pixel to fill in current stripe
		int drawStartY = -spriteHeight / 2 + height / 2 + vMoveScreen;
		if(drawStartY < 0) drawStartY = 0;
		int drawEndY = spriteHeight / 2 + height / 2 + vMoveScreen;
		if(drawEndY >= height) drawEndY = height - 1;

		//calculate width of the sprite
		int spriteWidth = (int)fabs((height / transformY) / uDiv);
		int drawStartX = -spriteWidth / 2 + spriteScreenX;
		if(drawStartX < 0) drawStartX = 0;
		int drawEndX = spriteWidth / 2 + spriteScreenX;
		if(drawEndX >= width) drawEndX = width - 1;

		//loop through every vertical stripe of the sprite on screen
		for(int stripe = drawStartX; stripe < drawEndX; stripe++)
		{
			int texX = (int)((256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * texWidth / spriteWidth) / 256);
			//the conditions in the if are:
			//1) it's in front of camera plane so you don't see things behind you
			//2) it's on the screen (left)
			//3) it's on the screen (right)
			//4) ZBuffer, with perpendicular distance
			if(transformY > 0 && stripe > 0 && stripe < width && transformY < info->z_buffer[stripe])
			for(int y = drawStartY; y < drawEndY; y++) //for every pixel of the current stripe
			{
				int d = (y-vMoveScreen) * 256 - height * 128 + spriteHeight * 128; //256 and 128 factors to avoid floats
				int texY = ((d * texHeight) / spriteHeight) / 256;
				int color = info->texture[info->sprite[spriteOrder[i]].texture][texWidth * texY + texX]; //get current color from the texture
				if((color & 0x00FFFFFF) != 0) info->buffer[y][stripe] = color; //paint pixel if it isn't black, black is the invisible color
			}
		}
	}
}

int	main_loop(t_info *info)
{
	floor_casting(info, &info->ray);
	calc(info);
	draw(info);
	return (0);
}

int	main(void)
{
	t_info	info;
	info.mlx = mlx_init();
	info.win = mlx_new_window(info.mlx, width, height, "yohlee's cub3d");
	init(&info);
	init_buffer(&info);
	if (init_texture(&info) == -1)
		return (-1);
	load_texture(&info);

	int	tmp[mapWidth][mapHeight] =
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

	memcpy(info.map, tmp, mapHeight * mapWidth);

	struct Sprite tmp2[numSprites] = 
	{
		{20.5, 11.5, 10}, //green light in front of playerstart
		//green lights in every room
		{18.5,4.5, 10},
		{10.0,4.5, 10},
		{10.0,12.5,10},
		{3.5, 6.5, 10},
		{3.5, 20.5,10},
		{3.5, 14.5,10},
		{14.5,20.5,10},

		//row of pillars in front of wall: fisheye test
		{18.5, 10.5, 9},
		{18.5, 11.5, 9},
		{18.5, 12.5, 9},

		//some barrels around the map
		{21.5, 1.5, 8},
		{15.5, 1.5, 8},
		{16.0, 1.8, 8},
		{16.2, 1.2, 8},
		{3.5,  2.5, 8},
		{9.5, 15.5, 8},
		{10.0, 15.1,8},
		{10.5, 15.8,8},
	};

	memcpy(info.sprite, tmp2, numSprites);

	info.img.img = mlx_new_image(info.mlx, width, height);
	info.img.data = (int *)mlx_get_data_addr(info.img.img, &info.img.bpp, &info.img.size_l, &info.img.endian);

	mlx_loop_hook(info.mlx, &main_loop, &info);
	mlx_hook(info.win, X_EVENT_KEY_PRESS, 0, &key_press, &info);

	mlx_loop(info.mlx);
}