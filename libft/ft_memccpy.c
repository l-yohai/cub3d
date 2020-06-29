/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 14:26:58 by yohlee            #+#    #+#             */
/*   Updated: 2020/02/27 19:32:33 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
**	copies bytes from src to dst
**	if (unsigned char)c occurs in src,
**	copy stops and byte after c in dst is returned.
*/

void	*ft_memccpy(void *dst, const void *src, int c, size_t len)
{
	unsigned char	*dst_str;
	unsigned char	*src_str;

	src_str = (unsigned char *)src;
	dst_str = (unsigned char *)dst;
	while (len--)
	{
		*dst_str = *src_str;
		if (*src_str == (unsigned char)c)
			return (++dst_str);
		dst_str++;
		src_str++;
	}
	return (NULL);
}
