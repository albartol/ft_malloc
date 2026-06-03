/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albartol <albartol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 14:39:54 by albartol          #+#    #+#             */
/*   Updated: 2024/06/14 01:10:07 by albartol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

static int	count_itoa(long nbr)
{
	int	i;

	i = 0;
	if (nbr < 0)
	{
		nbr = -nbr;
		i++;
	}
	if (nbr == 0)
		i++;
	while (nbr % 10 || nbr / 10)
	{
		nbr = nbr / 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(const int n)
{
	long	nbr;
	int		i;
	char	*res;

	nbr = (long)n;
	i = count_itoa(nbr);
	res = (char *)ft_calloc(i + 1, sizeof(char));
	i--;
	if (!res)
		return (0);
	if (nbr < 0)
	{
		res[0] = '-';
		nbr = -nbr;
	}
	if (nbr == 0)
		res[i--] = nbr % 10 + 48;
	while (nbr % 10 || nbr / 10)
	{
		res[i--] = nbr % 10 + 48;
		nbr = nbr / 10;
	}
	return (res);
}

/* #include <stdio.h>

int	main(void)
{
	printf("%s\n", ft_itoa(1234));
	printf("%s\n", ft_itoa(0));
	printf("%s\n", ft_itoa(-9));
	printf("%s\n", ft_itoa(-1234));
	printf("%s\n", ft_itoa(10));
	return (0);
} */