/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hguo <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 11:05:31 by hguo              #+#    #+#             */
/*   Updated: 2025/05/04 17:50:01 by hguo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 100
# endif

# include <stdlib.h>
# include <unistd.h>

char	*get_next_line(int fd);
char	*read_and_save(int fd, char *save);
char	*pick_line(char *save);
char	*clean_save(char *save);

char	*ft_strchr(const char *str, int c);
size_t	ft_strlen(char const *str);
char	*ft_strjoin(char *s1, char *s2);

#endif
