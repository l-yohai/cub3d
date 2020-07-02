/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nbrlen_base.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/07 18:35:26 by yohlee            #+#    #+#             */
/*   Updated: 2020/04/07 19:11:32 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_nbrlen_base(long long n, long long base_len)
{
	int	len;

	len = 1;
	if (n < 0)
		n *= -1;
	while (n >= base_len)
	{
		n /= base_len;
		len++;
	}
	return (len);
}
