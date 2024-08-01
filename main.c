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
		printf("Parsing error!\n");
	else if(error_type == ERR_MALLOC)
		printf("Malloc error!\n");
	ft_free(cube->gc, NULL);
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
		if (line[i] == ',')
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
	cube->prs->next_line_counter++;
	while(line && param_counter < 6)
	{
		if(line[0] == '\n')
			;
		else
			param_counter += check_param(cube, line);
		line = get_next_line(cube, fd);
		cube->prs->next_line_counter++;
	}
}

void	parse_map(t_cube *cube,int fd)
{
	char	*line;
	int		i;

	i = -1;
	line = get_next_line(cube, fd);

	while(line[0] == '\n')
	{
		line = get_next_line(cube, fd);
		cube->prs->next_line_counter++;
	}
	while(line)
	{
		i = -1;
		if(line[0] == '\n')
			clean_exit(cube, ERR_PARSING);
		while (line[++i] != '\n' && line[i] != '\0')
		{
			if(line[i] == 'N'|| line[i] == 'S'|| line[i] == 'W'|| line[i] == 'E')
				if(cube->prs->already_seen++)
					clean_exit(cube, ERR_PARSING);
			if(line[i] != '0' && line[i] != '1' && line[i] != ' ' && line[i] != 'N' && line[i] != 'S' && line[i] != 'W' && line[i] != 'E')
				clean_exit(cube, ERR_PARSING);
		}
		line[i] = '\0';
		if(ft_strlen(line) > (size_t)cube->prs->max_line_size)
			cube->prs->max_line_size = ft_strlen(line);
		cube->prs->n_lines++;
		line = get_next_line(cube, fd);
	}
	//Needs to be rewritten, sorry Maxime dont get DISCOMBOBULATED :)!
	close(fd);
	fd = open(cube->map_file, O_RDONLY);
	int **map = ft_malloc(cube, (cube->prs->n_lines + 3) * sizeof(int*)); //malloc 2d array of ints plus line -1 incapsulating the map
	for(int i = 0; i < cube->prs->n_lines + 2; i++)
		map[i] = ft_malloc(cube, (cube->prs->max_line_size + 3) * sizeof(int)); //malloc for each slot of the array the maximum line len found
 	for (int i = 0; i < cube->prs->n_lines + 2; i++)
  		for (int j = 0; (size_t)j < (size_t)cube->prs->max_line_size + 2; j++) //set all values at -1
    		map[i][j] = -1;
  	int k = -1;
	while(++k <= cube->prs->next_line_counter) //closes, reopens file and jups al the way at the start of the map
		line = get_next_line(cube, fd);
	int o = 0;
	while(line) //fill the malloced array with an ofset of 1 so we can have -1 on the borders
	{
		for (int j = 0; j < cube->prs->max_line_size; j++)
		{
			if(line[j] == ' ')
			{
				line[j] = -1;
				continue;
			}
			if(line[j] == '\n' || line[j] == '\0')
				break;
			map[o + 1][j + 1] = (int)line[j];
		}
		o++;
		line = get_next_line(cube, fd);
		if(!line || line[0] == '\n') //continue this loop as long as line is not null or line[0] = '\n'. im not sure if we consider new lines after the map as valid or not.
			break;
	}
	//		loop to print the map
	// for (int i = 0; i < cube->prs->n_lines + 2; i++)
 //  	{
 //  		for (int j = 0; (size_t)j < (size_t)cube->prs->max_line_size + 2; j++)
 //    		printf("%d", map[i][j]);
 //    	printf("\n");
 //   }
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
	ft_free(cube->gc, &cube->prs);
}

void cube_init(t_cube *cube, char *map_file)
{
	int i;

	i = -1;
	cube->gc = NULL;
	cube->map_file = map_file;
	cube->prs = ft_malloc(cube, sizeof(t_parse)); //can be freed after parsing;
	cube->prs->next_line_counter = 0;
	while(++i < 2)
		cube->colors[i] = -1;
	i = -1;
	while(++i < 4)
		cube->paths[i][0] = '\0';
	cube->prs->max_line_size = 0;
	cube->prs->already_seen = 0;
	cube->prs->n_lines = 0;
}

int	main(int argc, char *argv[])
{
	t_cube cube;

	if (argc != 2)
		return (write(1, "Error\n", 6), 1); //great success right?
	cube_init(&cube, argv[1]);
	parse(&cube);
	ft_free(cube.gc, NULL);
	return (0);
}
