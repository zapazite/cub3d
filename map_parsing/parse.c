/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-prin <mde-prin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 11:55:21 by efaiz             #+#    #+#             */
/*   Updated: 2024/08/08 10:35:23 by mde-prin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	parse_map(t_cube *cube, int fd)
{
	char	*line;
	int		x;

	line = get_next_line(cube, fd);
	if (!line)
		clean_exit(cube, ERR_PARSING);
	while (line[0] == '\n')
		line = get_next_line(cube, fd);
	while (line)
	{
		x = -1;
		check_line(cube, &x, line);
		line[x] = '\0';
		if ((int)ft_strlen(line) > cube->map_w)
			cube->map_w = (int)ft_strlen(line);
		add_line(cube, line);
		cube->map_h++;
		line = get_next_line(cube, fd);
	}
	fill_map(cube);
	cube->prs->min_x = cube->map_h;
	cube->prs->min_y = cube->map_w;
	check_spawn(cube, cube->player_x.hi, cube->player_y.hi);
	cube->player_x.hi -= cube->prs->min_x;
	cube->player_y.hi -= cube->prs->min_y;
}

void	fill_map(t_cube *cube)
{
	int	x;
	int	y;

	x = cube->map_h;
	cube->prs->prs_map = (int **)ft_malloc(cube, sizeof(int *) * cube->map_h);
	while (x--)
	{
		y = -1;
		cube->prs->prs_map[x] = (int *)ft_malloc(cube, sizeof(int) * cube->map_w);
		while (++y < (int)ft_strlen(cube->lines->line))
		{
			cube->prs->prs_map[x][y] = cube->lines->line[y];
			if (cube->prs->prs_map[x][y] == 'N' || cube->prs->prs_map[x][y] == 'S'
				|| cube->prs->prs_map[x][y] == 'W' || cube->prs->prs_map[x][y] == 'E')
			{
				cube->player_x.hi = x;
				cube->player_y.hi = y;
			}
		}
		while (y < cube->map_w)
			cube->prs->prs_map[x][y++] = ' ';
		cube->lines = cube->lines->next;
	}
	if (cube->player_x.i == -1)
		clean_exit(cube, ERR_PARSING);
}

void	parse_parameter(t_cube *cube, int fd)
{
	char	*line;
	int		param_counter;

	line = NULL;
	param_counter = 0;
	line = get_next_line(cube, fd);
	while (line && param_counter < 6)
	{
		if (line[0] == '\n')
			;
		else
			param_counter += check_param(cube, line);
		line = get_next_line(cube, fd);
	}
}

void	parse(t_cube *cube)
{
	int	fd;

	fd = open(cube->prs->map_file, O_RDONLY);
	if (fd < 0)
		clean_exit(cube, ERR_PARSING);
	parse_parameter(cube, fd);
	parse_map(cube, fd);
	close(fd);
}
