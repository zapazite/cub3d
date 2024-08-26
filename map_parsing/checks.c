/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efaiz <efaiz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 11:55:18 by efaiz             #+#    #+#             */
/*   Updated: 2024/08/23 13:14:19 by efaiz            ###   ########.fr       */
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
			cube->txt->paths[index][i] = line[i];
		cube->txt->paths[index][i] = '\0';
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
			&& cube->txt->paths[j][0] == '\0')
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
		{
			if (BONUS && line[(*i)] != 'H')
				clean_exit(cube, ERR_PARSING);
			else if (!BONUS)
				clean_exit(cube, ERR_PARSING);
		}
	}
}

void	check_map(t_cube *cube, int x, int y)
{
	if (x >= cube->map_h || x < 0 || y < 0 || y >= cube->map_w
		|| cube->prs->prs_map[x][y] == ' ')
		clean_exit(cube, ERR_PARSING);
	if (cube->prs->max_x < x)
		cube->prs->max_x = x;
	if (cube->prs->min_x > x)
		cube->prs->min_x = x;
	if (cube->prs->max_y < y)
		cube->prs->max_y = y;
	if (cube->prs->min_y > y)
		cube->prs->min_y = y;
	if (cube->prs->prs_map[x][y] == '1')
		return (cube->prs->prs_map[x][y] = '#', (void)0);
	if (cube->prs->prs_map[x][y] == '!' || cube->prs->prs_map[x][y] == '#'
		|| cube->prs->prs_map[x][y] == CLOSE_DOOR)
		return ;
	if (cube->prs->prs_map[x][y] == 'H')
		cube->prs->prs_map[x][y] = CLOSE_DOOR;
	else
		cube->prs->prs_map[x][y] = '!';
	check_map(cube, x - 1, y);
	check_map(cube, x + 1, y);
	check_map(cube, x, y - 1);
	check_map(cube, x, y + 1);
}
