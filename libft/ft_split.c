/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 17:24:30 by gdosch            #+#    #+#             */
/*   Updated: 2024/10/20 17:24:32 by gdosch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**ft_free(char **arr, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	return (NULL);
}

static size_t	ft_count_strs(char const *s, char c)
{
	size_t	i;
	size_t	str_count;

	i = 0;
	str_count = 0;
	while (s[i])
	{
		if ((s[i] != c && i == 0) || (s[i] != c && s[i - 1] == c))
			str_count++;
		i++;
	}
	return (str_count);
}

static char	**ft_fill_strs(char **arr, char const *s, char c)
{
	size_t	i;
	size_t	j;
	size_t	str_start;

	i = 1;
	j = 0;
	str_start = 0;
	while (i <= ft_strlen(s))
	{
		if (s[i] != c && s[i - 1] == c)
			str_start = i;
		else if ((s[i] == c || s[i] == '\0') && s[i - 1] != c)
		{
			arr[j] = ft_substr(s, str_start, i - str_start);
			if (!arr[j])
				return (ft_free(arr, j));
			j++;
		}
		i++;
	}
	return (arr);
}

char	**ft_split(char const *s, char c)
{
	char	**arr;

	if (!s)
		return (NULL);
	arr = ft_calloc((ft_count_strs(s, c) + 1), sizeof(char *));
	if (!arr)
		return (NULL);
	return (ft_fill_strs(arr, s, c));
}
