/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-prin <mde-prin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 10:35:49 by mde-prin          #+#    #+#             */
/*   Updated: 2024/08/08 09:57:25 by mde-prin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "gc/gc.h"
#include "get_next_line/get_next_line_bonus.h"
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

void	clean_exit(t_cube *cube, int error_type)
{
	if (error_type == ERR_PARSING)
		printf("Error\n");
	else if (error_type == ERR_MALLOC)
		printf("Malloc error!\n");
	ft_free_gc(cube->gc);
	exit(1);
}

void	cube_init(t_cube *cube, t_parse *prs, t_mlx *mlx, char *map_file)
{
	int	i;

	i = -1;
	cube->prs = prs;
	cube->mlx = mlx;
	cube->gc = NULL;
	cube->prs->max_x = 0;
	cube->prs->max_y = 0;
	cube->map_h = 0;
	cube->map_w = 0;
	cube->player_x.i = -1;
	cube->player_y.i = -1;
	cube->lines = NULL;
	cube->prs->map_file = map_file;
	while (++i < 2)
		cube->colors[i] = -1;
	i = -1;
	while (++i < 4)
		cube->paths[i][0] = '\0';
}

int	main(int argc, char *argv[])
{
	t_cube	cube;
	t_parse prs;
	t_mlx	mlx;

	if (argc != 2)
		return (write(1, "Error\n", 6), 1);
	cube_init(&cube, &prs, &mlx, argv[1]);
	parse(&cube);
	render(&cube);
	ft_free_gc(cube.gc);
	return (0);
}
