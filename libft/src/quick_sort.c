/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quick_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albartol <albartol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 12:47:32 by albartol          #+#    #+#             */
/*   Updated: 2024/06/14 01:10:20 by albartol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static int	quick_split(int *list, int start, int len)
{
	int	pivot;
	int	i;
	int	j;
	int	temp;

	pivot = list[(start + len) / 2];
	i = start;
	j = len;
	while (1)
	{
		while (list[i] < pivot)
			i++;
		while (list[j] > pivot)
			j--;
		if (i >= j)
			return (j);
		temp = list[i];
		list[i] = list[j];
		list[j] = temp;
	}
}

void	quick_sort(int *list, int start, int len)
{
	int	split;

	if (len - start <= 1)
		return ;
	split = quick_split(list, start, len);
	quick_sort(list, start, split);
	quick_sort(list, split, len);
}

/* int main(void)
{
    int i[] = {1,5,8,2,0,7,9,3,4,6};
    int n = 0;

    quick_sort(i, 0, 10 - 1);
    while (n < 10)
        ft_printf("%d", i[n++]);
    return (0);
} */