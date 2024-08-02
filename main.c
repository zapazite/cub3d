/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-prin <mde-prin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 10:35:49 by mde-prin          #+#    #+#             */
/*   Updated: 2024/08/01 12:21:42 by mde-prin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "gc/gc.h"
#include "get_next_line/get_next_line_bonus.h"
#include <string.h>
#include <fcntl.h>
#include <stdio.h>

int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	return (0);
}

void	clean_exit(t_cube *cube, int error_type)
{
	//might be more error types int he future right?
	if(error_type == ERR_PARSING)
		printf("Error\n");
	else if(error_type == ERR_MALLOC)
		printf("Malloc error!\n");
	ft_free_gc(cube->gc);
	//Destroy mlx stuff for later right?
	exit(1);
}

int	ft_strncmp(const char *str1, const char *str2, size_t n)
{
	int	i;

	i = -1;
	if (n == 0)
		return (0);
	while (n-- > 0)
	{
		if (str1[++i] != *str2)
			return ((unsigned char)str1[i] - *(unsigned char *)str2);
		str2++;
		if (str1[i] == '\0')
			return (0);
	}
	return (0);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	check_path(t_cube *cube, char *line, int index)
{
	int	i;
	int fd;

	i = 0;
	while(*line == ' ')
		line++;
	while (line[i] != '\n' && line[i] != '\0')
		i++;
	line[i] = '\0';
	fd = open(line, O_RDONLY);
	if(fd == -1)
		clean_exit(cube, ERR_PARSING); //to be written
	else
	{
		close(fd);
		i = -1;
		while(line[++i])
			cube->paths[index][i] = line[i];
		cube->paths[index][i] = '\0';
	}
	return (1);
}

int rgb_atoi(const char *nptr)
{
	int	n;

	n = 0;
	if(!ft_isdigit(*nptr))
		return (-1);
	while (ft_isdigit(*nptr))
	{
		n = n * 10 + (*nptr++ - '0');
		if (n > 255)
			return (-1);
	}
	return (n);
}

int	check_rgb(t_cube *cube, char *line, int index)
{
	int	i;
	int color_count;
	int color_component;

	i = 0;
	color_count = 0;
	while(*line == ' ')
		line++;
	while (line[i] != '\n' && line[i] != '\0')
	{
		color_component = rgb_atoi(line + i);
		if (color_component < 0)
			clean_exit(cube, ERR_PARSING);
		color_count++;
		if(color_count > 3)
			clean_exit(cube, ERR_PARSING);
		cube->colors[index] = (cube->colors[index] << 8) | color_component;
		while (ft_isdigit(line[i]))
			i++;
		if(line[i] == ',' && color_count < 3)
			i++;
	}
	return (1);
}

int		check_param(t_cube *cube, char *line)
{
	int j;
	int success;
	const char	*param_array[] = {"NO ", "SO ", "WE ", "EA "};
	const char	*color_array[] = {"F ", "C "};

	j = -1;
	success = 0;

	while(++j < 4)
		if(!ft_strncmp(line, param_array[j], ft_strlen(param_array[j])) && cube->paths[j][0] == '\0')
			success = check_path(cube, line + 3, j);
	j = -1;
	while(++j < 2 && !success)
		if(!ft_strncmp(line, color_array[j], ft_strlen(color_array[j])) && cube->colors[j] == -1)
			success = check_rgb(cube, line + 2, j);
	if (!success)
		clean_exit(cube, ERR_PARSING);
	return (1);
}

void	parse_parameter(t_cube *cube, int fd)
{
	char	*line;
	int		param_counter;

	line = NULL;
	param_counter = 0;
	line = get_next_line(cube, fd);
	while(line && param_counter < 6)
	{
		if(line[0] == '\n')
			;
		else
			param_counter += check_param(cube, line);
		line = get_next_line(cube, fd);
	}
}

void	add_line(t_cube *cube, char *line)
{
	t_lines *new_node;

	new_node = ft_malloc(cube, sizeof(t_lines));
	new_node->line = line;
	new_node->next = cube->lines;
	cube->lines = new_node;
}

void	check_line(t_cube *cube, int *i, char *line)
{
	int spawn_flag;

	spawn_flag = 0;
	if(line[0] == '\n')
		clean_exit(cube, ERR_PARSING);
	while(line[++(*i)] != '\n' && line[(*i)] != '\0')
	{
		if(line[(*i)] == 'N'|| line[(*i)] == 'S'|| line[(*i)] == 'W'|| line[(*i)] == 'E')
		{
			if(spawn_flag++)
				clean_exit(cube, ERR_PARSING);
		}
		else if(line[(*i)] != '0' && line[(*i)] != '1' && line[(*i)] != ' ')
			clean_exit(cube, ERR_PARSING);
	}
}

void	fill_map(t_cube *cube)
{
	int	i;
	int	j;

	i = cube->map_h;
	cube->map = (int **)ft_malloc(cube, sizeof(int*) * cube->map_h);
	while(i--)
	{
		j = -1;
		cube->map[i] = (int*)ft_malloc(cube, sizeof(int) * cube->map_w);
		while(++j < (int)ft_strlen(cube->lines->line))
		{
			cube->map[i][j] = cube->lines->line[j];
			if(cube->map[i][j] == 'N'|| cube->map[i][j] == 'S'|| cube->map[i][j] == 'W'|| cube->map[i][j] == 'E')
			{
				cube->spawn_x = i;
				cube->spawn_y = j;
			}
		}
		while(j < cube->map_w)
			cube->map[i][j++] = ' ';
		cube->lines = cube->lines->next;
	}
}

void 	valid_spawn(t_cube *cube, int x, int y)
{
	if(x >= cube->map_h || x < 0 || y < 0 || y >= cube->map_w || cube->map[x][y] == ' ')
		clean_exit(cube, ERR_PARSING);
	if(cube->map[x][y] == '1' || cube->map[x][y] == '!')
		return ;
	cube->map[x][y] = '!';
	valid_spawn(cube, x - 1, y);
	valid_spawn(cube, x + 1, y);
	valid_spawn(cube, x, y - 1);
	valid_spawn(cube, x, y + 1);
}

void	parse_map(t_cube *cube, int fd)
{
	char			*line;
	int				i;

	line = get_next_line(cube, fd);
	if(!line)
		clean_exit(cube, ERR_PARSING);
	while(line[0] == '\n')
		line = get_next_line(cube, fd);
	while(line)
	{
		i = -1;
		check_line(cube, &i, line);
		line[i] = '\0';
		if((int)ft_strlen(line) > cube->map_w)
			cube->map_w = (int)ft_strlen(line);
		add_line(cube, line);
		cube->map_h++;
		line = get_next_line(cube, fd);
	}
	fill_map(cube);
	if(cube->spawn_x == -1)
		clean_exit(cube, ERR_PARSING);
	valid_spawn(cube, cube->spawn_x, cube->spawn_y);
}


void	parse(t_cube *cube)
{
	int		fd;
	fd = open(cube->map_file, O_RDONLY);
	if (fd < 0)
		clean_exit(cube, ERR_PARSING);
	parse_parameter(cube, fd);
	parse_map(cube, fd);
	close (fd);
	// for (int i = 0; i < cube->map_h; i++)
	// {
 //  			for (int j = 0; j < cube->map_w; j++)
 //    			printf("%c", cube->map[i][j]);
 //    	printf("\n");
 //   	}
}

void cube_init(t_cube *cube, char *map_file)
{
	int i;

	i = -1;
	cube->gc = NULL;
	cube->map_h = 0;
	cube->map_w = 0;
	cube->lines = NULL;
	cube->spawn_x = -1;
	cube->spawn_y = -1;
	cube->map_file = map_file;
	while(++i < 2)
		cube->colors[i] = -1;
	i = -1;
	while(++i < 4)
		cube->paths[i][0] = '\0';
}

int	main(int argc, char *argv[])
{
	t_cube cube;

	if (argc != 2)
		return (write(1, "Error\n", 6), 1);
	cube_init(&cube, argv[1]);
	parse(&cube);
	ft_free_gc(cube.gc);
	return (0);
}
