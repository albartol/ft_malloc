/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albartol <albartol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 13:21:43 by albartol          #+#    #+#             */
/*   Updated: 2024/06/14 01:10:07 by albartol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

char	*ft_strdup(const char *str)
{
	size_t	len;
	char	*dup;

	len = ft_strlen(str);
	dup = (char *)malloc(len + 1);
	if (!dup)
		return (0);
	ft_strlcpy(dup, str, len + 1);
	return (dup);
}

/* #include <stdio.h>
#include <string.h>

int main (void)
{
   char *a = "megalovanina";
   char *dup1 = strdup(a);
   char *dup2 = ft_strdup(a);

   printf("%s\n", a);
   printf("%s\n", dup1);
   free(dup1);
   printf("%s\n", dup2);
   free(dup2);
   return(0);
} */
