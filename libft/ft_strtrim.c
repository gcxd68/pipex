/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 17:24:08 by gdosch            #+#    #+#             */
/*   Updated: 2024/10/20 17:24:10 by gdosch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(const char *s1, const char *set)
{
	size_t	start_len;
	size_t	end_len;

	if (!s1 || !set)
		return (NULL);
	if (ft_strlen(s1) == 0)
		return (ft_strdup(""));
	start_len = 0;
	while (ft_strchr(set, s1[start_len]) != NULL)
		start_len++;
	end_len = 0;
	while (ft_strchr(set, s1[ft_strlen(s1) - end_len - 1]) != NULL)
		end_len++;
	return (ft_substr(s1, start_len, ft_strlen(s1) - start_len - end_len));
}
