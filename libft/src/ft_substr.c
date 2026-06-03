/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albartol <albartol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 17:39:48 by albartol          #+#    #+#             */
/*   Updated: 2024/06/14 01:10:07 by albartol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*str;
	unsigned int	i;

	if (start >= ft_strlen(s))
		str = (char *)ft_calloc(1, 1);
	else if (len > ft_strlen(s) - start)
		str = (char *)ft_calloc(ft_strlen(s) - start + 1, 1);
	else
		str = (char *)ft_calloc(len + 1, 1);
	if (!str)
		return (0);
	i = 0;
	if (start >= ft_strlen(s))
		return (str);
	while (len-- && s[start])
		str[i++] = s[start++];
	return (str);
}

/* #include <stdio.h>

int main (void)
{
   char *a = "megalovanina";
   char *sub = ft_substr(a,3,5);

   printf("%s\n", a);
   printf("%s\n", sub);
   free(sub);
   return(0);
} */