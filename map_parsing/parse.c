/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-prin <mde-prin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 11:55:21 by efaiz             #+#    #+#             */
/*   Updated: 2024/08/06 11:53:22 by mde-prin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	parse_map(t_cube *cube, int fd)
{
	char	*line;
	int		i;

	line = get_next_line(cube, fd);
	if (!line)
		clean_exit(cube, ERR_PARSING);
	while (line[0] == '\n')
		line = get_next_line(cube, fd);
	while (line)
	{
		i = -1;
		check_line(cube, &i, line);
		line[i] = '\0';
		if ((int)ft_strlen(line) > cube->map_w)
			cube->map_w = (int)ft_strlen(line);
		add_line(cube, line);
		cube->map_h++;
		line = get_next_line(cube, fd);
	}
	fill_map(cube);
	cube->prs->min_x = cube->map_h;
	cube->prs->min_y = cube->map_w;
	check_map(cube, cube->player_x, cube->player_y);
	cube->player_x -= cube->prs->min_x;
	cube->player_y -= cube->prs->min_y;
}

void    check_spawn(t_cube *cube, int x, int y)
{
    if (cube->prs->prs_map[x][y] == 'S')
    {
        cube->player_x = x;
        cube->player_y = y;
    }
    else if (cube->prs->prs_map[x][y] == 'E')
    {
        cube->player_x = x;
        cube->player_y = y;
        cube->player_angle = PI / 2;
    }
    else if (cube->prs->prs_map[x][y] == 'N')
    {
        cube->player_x = x;
        cube->player_y = y;
        cube->player_angle = PI;
    }
    else if (cube->prs->prs_map[x][y] == 'W')
    {
        cube->player_x = x;
        cube->player_y = y;
        cube->player_angle = 3 * PI / 2;
    }
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
		    check_spawn(cube, x, y);
		}
		while (y < cube->map_w)
			cube->prs->prs_map[x][y++] = ' ';
		cube->lines = cube->lines->next;
	}
	if (cube->player_x == -1)
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
		if (line[0] != '\n')
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
