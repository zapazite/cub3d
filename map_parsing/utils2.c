/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efaiz <efaiz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 13:16:15 by efaiz             #+#    #+#             */
/*   Updated: 2024/08/23 13:17:00 by efaiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

int	rgb_atoi(const char *nptr)
{
	int	n;

	n = 0;
	if (!ft_isdigit(*nptr))
		return (-1);
	while (ft_isdigit(*nptr))
	{
		n = n * 10 + (*nptr++ - '0');
		if (n > 255)
			return (-1);
	}
	return (n);
}

void	add_line(t_cube *cube, char *line)
{
	t_lines	*new_node;

	new_node = ft_malloc(cube, sizeof(t_lines));
	new_node->line = line;
	new_node->next = cube->lines;
	cube->lines = new_node;
}
