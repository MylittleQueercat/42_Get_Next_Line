/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hguo <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 19:42:31 by hguo              #+#    #+#             */
/*   Updated: 2025/05/04 13:51:35 by hguo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
// #include <stdio.h>
// #include <fcntl.h>

char	*read_and_save(int fd, char *save)
{
	int		bytes_read;
	char	*buffer;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	bytes_read = 1;
	while (!ft_strchr(save, '\n') && bytes_read != 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(buffer);
			free(save);
			return (NULL);
		}
		buffer[bytes_read] = '\0';
		save = ft_strjoin(save, buffer);
	}
	free(buffer);
	return (save);
}

char	*pick_line(char *save)
{
	int		i;
	char	*line;

	i = 0;
	if (!save[i])
		return (NULL);
	while (save[i] && save[i] != '\n')
		i++;
	line = malloc(i + 2);
	if (!line)
		return (NULL);
	i = 0;
	while (save[i] && save[i] != '\n')
	{
		line[i] = save[i];
		i++;
	}
	if (save[i] == '\n')
	{
		line[i] = save[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

char	*clean_save(char *save)
{
	int		i;
	int		j;
	char	*new;

	i = 0;
	while (save[i] && save[i] != '\n')
		i++;
	if (!save[i])
	{
		free(save);
		return (NULL);
	}
	new = malloc(sizeof(char) *(ft_strlen(save) - i + 1));
	if (!new)
		return (NULL);
	i++;
	j = 0;
	while (save[i])
		new[j++] = save[i++];
	new[j] = '\0';
	free(save);
	return (new);
}

char	*get_next_line(int fd)
{
	static char	*save;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	save = read_and_save(fd, save);
	if (!save)
		return (NULL);
	line = pick_line(save);
	save = clean_save(save);
	return (line);
}

/*
int main(void)
{
	int fd = open("some_lines.txt", O_RDONLY);
	char *line;

	line = get_next_line(fd);
	printf("line1: %s", line);
	free(line);

	line = get_next_line(fd);
	printf("line2: %s", line);
	free(line);

	line = get_next_line(fd);
	printf("line3: %s", line);
	free(line);

	close(fd);
	return 0;
}*/
