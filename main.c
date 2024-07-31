/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-prin <mde-prin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 10:35:49 by mde-prin          #+#    #+#             */
/*   Updated: 2024/07/31 13:05:17 by mde-prin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int		check_

void	parse_parameter(t_cube *cube,int fd)
{
	char	*line;

	line = NULL;
	line = get_next_line(cube, fd);
	while(line)
	{
		if ()
			return ;
		line = get_next_line(cube, fd);
	}
}

void	parse_map(t_cube *cube,int fd)
{
	char	*line;

	line = NULL;
	line = get_next_line(cube, fd);
	while(line)
	{
		line = get_next_line(cube, fd);
	}
}

void	parse(t_cube *cube, char *map_file)
{
	int		fd;
	fd = open(map_file, O_RDONLY);
	if (fd < 0)
		clean_and_exit();
	parse_parameter(cube, fd);
	parse_map(cube, fd);
	
}

void cube_init(t_cube *cube)
{
	cube->gc = NULL;
}

int	main(int argc, char *argv[])
{
	t_cube cube;

	if (argc > 2)
		return (write(1, "Error\n", 6), 1);
	cube_init(&cube);
	parse(&cube, argv[1]);
	return (0);
}
