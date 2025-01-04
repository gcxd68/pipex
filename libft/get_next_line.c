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
	char	*tmp;
	size_t	line_len;
	size_t	chunk_len;
	size_t	i;

	i = 0;
	while ((*buffer)[i] && (*buffer)[i] != '\n')
		i++;
	line_len = ft_strlen(*line);
	chunk_len = i + ((*buffer)[i] == '\n');
	tmp = malloc(line_len + chunk_len + 1);
	if (!tmp)
		return (-1);
	if (*line)
		ft_memcpy(tmp, *line, line_len);
	ft_memcpy(tmp + line_len, *buffer, chunk_len);
	tmp[line_len + chunk_len] = '\0';
	free(*line);
	*line = tmp;
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
	line = malloc (1);
	if (!line)
		return (NULL);
	*line = '\0';
	ret = 1;
	while (ret > 0)
		ret = ft_parse_data(fd, buffer, &line);
	if (ret == -1 || !*line)
		return (free(line), line = 0, NULL);
	return (line);
}
