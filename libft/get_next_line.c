/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 15:15:45 by gdosch            #+#    #+#             */
/*   Updated: 2024/11/18 15:15:47 by gdosch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static ssize_t	ft_build_line(char **line, char **buffer)
{
	char			*tmp;
	const size_t	line_len = ft_strlen(*line);
	size_t			chunk_len;
	size_t			i;

	i = 0;
	while ((*buffer)[i] && (*buffer)[i] != '\n')
		i++;
	chunk_len = i + ((*buffer)[i] == '\n');
	tmp = ft_realloc(*line, line_len + 1, line_len + chunk_len + 1);
	if (!tmp)
		return (-1);
	*line = tmp;
	ft_strlcpy(*line + line_len, *buffer, chunk_len + 1);
	return (i);
}

static ssize_t	ft_parse_data(int fd, char *buffer, char **line)
{
	ssize_t	ret;

	if (!*buffer)
	{
		ret = read(fd, buffer, BUFFER_SIZE);
		if (ret <= 0)
			return (ret);
		buffer[ret] = '\0';
	}
	ret = ft_build_line(line, &buffer);
	if (ret < 0)
		return (-1);
	if (buffer[ret] == '\0')
		return (((*buffer) = '\0'), 1);
	ft_memcpy(buffer, buffer + ret + 1, ft_strlen(buffer) - ret);
	return (0);
}

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE + 1];
	char		*line;
	int			ret;

	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	line = ft_calloc(1, 1);
	if (!line)
		return (NULL);
	ret = 1;
	while (ret > 0)
		ret = ft_parse_data(fd, buffer, &line);
	if (ret == -1 || !*line)
		return (free(line), line = 0, NULL);
	return (line);
}
