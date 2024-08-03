/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efaiz <efaiz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 11:55:18 by efaiz             #+#    #+#             */
/*   Updated: 2024/08/03 11:55:19 by efaiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

int	check_path(t_cube *cube, char *line, int index)
{
	int	i;
	int	fd;

	i = 0;
	while (*line == ' ')
		line++;
	while (line[i] != '\n' && line[i] != '\0')
		i++;
	line[i] = '\0';
	fd = open(line, O_RDONLY);
	if (fd == -1)
		clean_exit(cube, ERR_PARSING);
	else
	{
		close(fd);
		i = -1;
		while (line[++i])
			cube->paths[index][i] = line[i];
		cube->paths[index][i] = '\0';
	}
	return (1);
}

int	check_rgb(t_cube *cube, char *line, int index)
{
	int	i;
	int	color_count;
	int	color_component;

	i = 0;
	color_count = 0;
	while (*line == ' ')
		line++;
	while (line[i] != '\n' && line[i] != '\0')
	{
		color_component = rgb_atoi(line + i);
		if (color_component < 0)
			clean_exit(cube, ERR_PARSING);
		color_count++;
		if (color_count > 3)
			clean_exit(cube, ERR_PARSING);
		cube->colors[index] = (cube->colors[index] << 8) | color_component;
		while (ft_isdigit(line[i]))
			i++;
		if (line[i] == ',' && color_count < 3)
			i++;
	}
	return (1);
}

int	check_param(t_cube *cube, char *line)
{
	int			j;
	int			success;
	const char	*param_array[] = {"NO ", "SO ", "WE ", "EA "};
	const char	*color_array[] = {"F ", "C "};

	j = -1;
	success = 0;
	while (++j < 4)
		if (!ft_strncmp(line, param_array[j], ft_strlen(param_array[j]))
			&& cube->paths[j][0] == '\0')
			success = check_path(cube, line + 3, j);
	j = -1;
	while (++j < 2 && !success)
		if (!ft_strncmp(line, color_array[j], ft_strlen(color_array[j]))
			&& cube->colors[j] == -1)
			success = check_rgb(cube, line + 2, j);
	if (!success)
		clean_exit(cube, ERR_PARSING);
	return (1);
}

void	check_line(t_cube *cube, int *i, char *line)
{
	static int	spwan_flag;

	if (line[0] == '\n')
		clean_exit(cube, ERR_PARSING);
	while (line[++(*i)] != '\n' && line[(*i)] != '\0')
	{
		if (line[(*i)] == 'N' || line[(*i)] == 'S' || line[(*i)] == 'W'
			|| line[(*i)] == 'E')
		{
			if (spwan_flag++)
				clean_exit(cube, ERR_PARSING);
		}
		else if (line[(*i)] != '0' && line[(*i)] != '1' && line[(*i)] != ' ')
			clean_exit(cube, ERR_PARSING);
	}
}

void	check_spawn(t_cube *cube, int x, int y)
{
	// printf("min x %d\n", cube->min_x);
	// printf("max x %d\n", cube->max_x);
	// printf("min y %d\n", cube->min_y);
	// printf("max y %d\n", cube->max_y);
	if (x >= cube->map_h || x < 0 || y < 0 || y >= cube->map_w
		|| cube->map[x][y] == ' ')
		clean_exit(cube, ERR_PARSING);

	if(cube->max_x < x)
		cube->max_x = x;

	if(cube->min_x > x)
		cube->min_x = x;

	if(cube->max_y < y)
		cube->max_y = y;

	if(cube->min_y > y)
		cube->min_y = y;

	if (cube->map[x][y] == '1')
	{
		cube->map[x][y] = '#';
		return;
	}
	if(cube->map[x][y] == '!' || cube->map[x][y] == '#')
	{
		return ;
	}
	cube->map[x][y] = '!';
	check_spawn(cube, x - 1, y);
	check_spawn(cube, x + 1, y);
	check_spawn(cube, x, y - 1);
	check_spawn(cube, x, y + 1);
}
