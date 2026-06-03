/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putptr_n.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albartol <albartol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 18:32:24 by albartol          #+#    #+#             */
/*   Updated: 2024/06/14 01:10:07 by albartol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

int	ft_putptr_n(unsigned long ptr)
{
	int	res;
	int	n;

	res = write(1, "0x", 2);
	if (res == -1)
		return (-1);
	n = ft_putnbr_base_n(ptr, "0123456789abcdef");
	if (n == -1)
		return (-1);
	res += n;
	return (res);
}
