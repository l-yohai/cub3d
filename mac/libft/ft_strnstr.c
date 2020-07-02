/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 14:27:26 by yohlee            #+#    #+#             */
/*   Updated: 2020/02/29 00:51:34 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	if (!*little)
		return ((char *)big);
	while (*big && len >= ft_strlen(little))
	{
		if (*big == *little && (ft_memcmp(big, little, ft_strlen(little)) == 0))
			return ((char *)big);
		big++;
		len--;
	}
	return (NULL);
}
