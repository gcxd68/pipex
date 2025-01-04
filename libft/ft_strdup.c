/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 13:27:15 by gdosch            #+#    #+#             */
/*   Updated: 2024/07/30 13:27:17 by gdosch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(char *s)
{
	char	*dup;
	char	*ptr;

	dup = (char *)malloc(1 * (ft_strlen(s) + 1));
	if (!dup)
		return (NULL);
	ptr = dup;
	while (*s)
		*dup++ = *s++;
	*dup = '\0';
	return (ptr);
}
