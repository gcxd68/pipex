/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 17:24:49 by gdosch            #+#    #+#             */
/*   Updated: 2024/10/20 17:24:51 by gdosch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_nlen(int *n, size_t *len)
{
	int	nbr;

	*len = 0;
	if (*n < 0)
	{
		*len += 1;
		*n -= 2 * *n;
	}
	nbr = *n;
	while (nbr > 0)
	{
		nbr /= 10;
		*len += 1;
	}
	return (*len);
}

static char	*ft_str_wr(int n, char *res, size_t len)
{
	while (n > 0)
	{
		res[len - 1] = n % 10 + '0';
		n /= 10;
		len--;
	}
	if (res[0] == 0)
		res[0] = '-';
	return (res);
}

char	*ft_itoa(int n)
{
	char	*res;
	size_t	len;

	if (n == 0)
		return (ft_strdup("0"));
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	res = ft_calloc(ft_nlen(&n, &len) + 1, sizeof(char));
	if (!res)
		return (NULL);
	return (ft_str_wr(n, res, len));
}
