/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albartol <albartol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 14:30:37 by albartol          #+#    #+#             */
/*   Updated: 2024/06/14 01:10:07 by albartol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

static char	*order_nl(char *str)
{
	char	*res;
	int		i;
	int		n;

	i = 0;
	n = 0;
	while (str[i] && str[i] != '\n')
		i++;
	if (!str[i])
		return (free(str), NULL);
	res = (char *)ft_calloc(ft_strlen(str) - i + 1, 1);
	if (!res)
		return (free(str), NULL);
	i++;
	while (str[i])
		res[n++] = str[i++];
	return (free(str), res);
}

static char	*substr_nl(char *str)
{
	char	*res;
	size_t	len;

	if (!str[0])
		return (0);
	len = 0;
	while (str[len] && str[len] != '\n')
		len++;
	if (str[len] == '\n')
		len++;
	res = (char *)ft_calloc(len + 1, sizeof(char));
	if (!res)
		return (0);
	ft_strlcpy(res, str, len + 1);
	return (res);
}

static char	*strjoin_nl(char *s1, char *s2)
{
	char	*str;
	size_t	len1;
	size_t	len2;

	if (!s1)
	{
		s1 = (char *)ft_calloc(1, sizeof(char));
		if (!s1)
			return (0);
	}
	if (!s2)
		return (free(s1), NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	str = (char *)ft_calloc(len1 + len2 + 1, sizeof(char));
	if (!str)
		return (free(s1), NULL);
	ft_strlcpy(str, s1, len1 + 1);
	ft_strlcpy(str + len1, s2, len2 + 1);
	return (free(s1), str);
}

static char	*read_nl(char *str, int fd)
{
	char	*res;
	ssize_t	data;

	res = (char *)ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (!res)
		return (free(str), NULL);
	data = 2;
	while (!ft_strchr(str, '\n') && data)
	{
		data = read(fd, res, BUFFER_SIZE);
		if (data == -1)
			return (free(res), free(str), NULL);
		if (data == 0)
			return (free(res), str);
		res[data] = 0;
		str = strjoin_nl(str, res);
		if (!str)
			return (free(res), str);
	}
	return (free(res), str);
}

char	*get_next_line(int fd)
{
	static char	*temp[OPEN_MAX];
	char		*res;

	if (fd < 0 || BUFFER_SIZE <= 0 || fd > OPEN_MAX)
		return (0);
	temp[fd] = read_nl(temp[fd], fd);
	if (!temp[fd])
		return (0);
	res = substr_nl(temp[fd]);
	if (!res)
		return (free(temp[fd]), temp[fd] = NULL, NULL);
	temp[fd] = order_nl(temp[fd]);
	return (res);
}

/* int	main(int argc, char **argv)
{
	int i = 50;
	int fd = open(argv[argc], O_RDONLY);
	char *res;
	while (i-- && --argc)
	{
		res = get_next_line(fd);
		printf("%s", res);
		if (res)
			free (res);
		fd = open(argv[argc], O_RDONLY);
	}
	return (0);
} */
