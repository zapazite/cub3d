/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-prin <mde-prin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 11:25:00 by mde-prin          #+#    #+#             */
/*   Updated: 2024/08/01 11:28:56 by mde-prin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	fill_lst(t_cube *cube, t_list **lst, t_list *last, int fd)
{
	char	*buffer;
	int		bytes_read;
	int		i;

	buffer = (char *)ft_malloc(cube, sizeof(char) * BUFFER_SIZE);
	while (!get_nl(*lst))
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read <= 0)
			break ;
		i = 0;
		if (!*lst)
			*lst = ft_lstnew(cube, buffer[i++]);
		if (!last)
			last = *lst;
		while (i < bytes_read)
		{
			last->next = ft_lstnew(cube, buffer[i]);
			last = last->next;
			i++;
		}
	}
}

char	*lst_to_str(t_cube *cube, t_list **lst)
{
	char	*line;
	int		len;
	int		i;

	if (*lst == NULL)
		return (NULL);
	len = get_len(*lst, 0);
	line = (char *)ft_malloc(cube, sizeof(char) * (len + 1));
	i = 0;
	while (i < len)
	{
		line[i] = (*lst)->letter;
		*lst = (*lst)->next;
		i++;
	}
	line[len] = '\0';
	return (line);
}

char	*get_next_line(t_cube *cube, int fd)
{
	static t_list	*lst[1024];

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	fill_lst(cube, &lst[fd], last(lst[fd]), fd);
	return (lst_to_str(cube, &lst[fd]));
}
