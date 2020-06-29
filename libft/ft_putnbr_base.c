/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/10 19:12:19 by yohlee            #+#    #+#             */
/*   Updated: 2020/03/14 16:25:35 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_base(int n, char *str)
{
	int	radix;

	radix = ft_strlen(str);
	if (!radix)
		return ;
	if (n >= 0)
	{
		if (n > 9)
			ft_putnbr_base(n / radix, str);
		ft_putchar(str[n % radix]);
	}
	else
	{
		ft_putchar('-');
		if (n < -9)
			ft_putnbr_base(n / (radix * (-1)), str);
		ft_putchar(str[n % radix * -1]);
	}
}
