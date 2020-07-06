/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/06 09:12:27 by yohlee            #+#    #+#             */
/*   Updated: 2020/07/06 09:29:19 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int		get_max_line_size(t_list *lst)
{
	int		max;
	int		size;

	max = 0;
	while (lst)
	{
		size = ft_strlen((char *)lst->content);
		if (size > max)
			max = size;
		lst = lst->next;
	}
	return (max);
}

void	sort(t_sprite *sprite, int num)
{
	int	i;
	int	j;
	int	max;
	t_sprite	temp;

	i = 0;
	max = 0;
	while (i < num - 1)
	{
		j = i + 1;
		while (j < num)
		{
			if (sprite[i].distance > sprite[i].distance)
				max = j;
			j++;
		}
		if (i != max)
		{
			temp = sprite[i];
			sprite[i] = sprite[max];
			sprite[max] = temp;
		}
		i++;
	}
}

int		set_pos_sprite(t_info *info, int i, int x, int y)
{
	info->sprite[i].x = 0.5f + x;
	info->sprite[i].y = 0.5f + y;
	return (1);
}

int		set_sprite(t_info *info)
{
	int	x;
	int	y;
	int	i;

	y = 0;
	i = 0;
	if (!(info->sprite =\
			(t_sprite *)malloc(sizeof(t_sprite) * info->num_sprite)))
		return (exit_error(info));
	while (y < info->map_height)
	{
		x = 0;
		while (x < info->map_width)
		{
			if (info->map[y][x] == '2')
			{
				set_pos_sprite(info, i, x, y);
				i++;
			}
			x++;
		}
		y++;
	}
	return (1);
}
