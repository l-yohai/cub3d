/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 14:27:05 by yohlee            #+#    #+#             */
/*   Updated: 2020/02/27 18:47:25 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t len)
{
	char	*dst_str;
	char	*src_str;

	if (dst == src || len == 0)
		return (dst);
	dst_str = (char *)dst;
	src_str = (char *)src;
	while (len--)
		*dst_str++ = *src_str++;
	return (dst);
}
