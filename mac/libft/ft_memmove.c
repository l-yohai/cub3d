/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 14:27:07 by yohlee            #+#    #+#             */
/*   Updated: 2020/04/12 23:36:56 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** dest >= src
** bwd copy
** dest < src
** fwd copy
*/

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char	*dst_str;
	unsigned char	*src_str;

	if (dst == src || !len)
		return (dst);
	dst_str = (unsigned char *)dst;
	src_str = (unsigned char *)src;
	if (dst_str < src_str)
	{
		while (len--)
			*dst_str++ = *src_str++;
	}
	else
	{
		while (--len)
			*(dst_str + len) = *(src_str + len);
		*(dst_str + len) = *(src_str + len);
	}
	return (dst);
}
