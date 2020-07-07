/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/05 07:34:30 by yohlee            #+#    #+#             */
/*   Updated: 2020/07/07 10:05:39 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	parse_map(t_info *info, t_list *lst)
{
	if (!init_map(info, lst))
		return (exit_error(info));
	if (!fill_map(info, lst))
		return (exit_error(info));
	ft_lstclear(&lst, free);
	if (!check_valid_map(info))
		return (exit_error(info));
	return (1);
}

int	read_map(t_info *info, char *line)
{
	t_list	*lst;

	info->map_width = 0;
	info->map_height = 0;
	lst = ft_lstnew(ft_strdup(line));
	free(line);
	while (get_next_line(info->fd, &line) > 0)
	{
		ft_lstadd_back(&lst, ft_lstnew(ft_strdup(line)));
		free(line);
	}
	ft_lstadd_back(&lst, ft_lstnew(ft_strdup(line)));
	free(line);
	if (!parse_map(info, lst))
		return (exit_error(info));
	return (1);
}

int	init_direction(t_info *info, int i, int j, int *dir_count)
{
	if (is_dir(info->map[i][j]) == 1)
	{
		info->dir = info->map[i][j];
		info->player.x = 0.5f + j;
		info->player.y = 0.5f + i;
		(*dir_count)++;
	}
	if (info->map[i][j] == '2')
		info->num_sprite++;
	return (1);
}

int	fill_map(t_info *info, t_list *lst)
{
	int		i;
	int		j;
	char	*line;
	int		dir_count;

	i = -1;
	dir_count = 0;
	while (++i < info->map_height)
	{
		j = -1;
		line = (char *)lst->content;
		while (++j < (int)ft_strlen(line))
		{
			if (is_map_arg(line[j]) == -1)
				return (exit_error(info));
			info->map[i][j] = line[j];
			init_direction(info, i, j, &dir_count);
		}
		lst = lst->next;
	}
	if (dir_count == 0 || dir_count > 1)
		return (exit_error(info));
	return (1);
}

int	check_valid_map(t_info *info)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (!check_map_top(info))
		return (exit_error(info));
	while (i < info->map_height - 1)
	{
		j = 0;
		if (!check_map_left(info, i, j))
			return (exit_error(info));
		while (j < (int)ft_strlen(info->map[i]))
		{
			if (!check_map_middle(info, i, j))
				return (exit_error(info));
			j++;
		}
		if (!check_map_right(info, i, j))
			return (exit_error(info));
		i++;
	}
	if (!check_map_bottom(info, i))
		return (exit_error(info));
	return (1);
}
