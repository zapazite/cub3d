/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efaiz <efaiz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 10:35:49 by mde-prin          #+#    #+#             */
/*   Updated: 2024/08/03 11:50:36 by efaiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <time.h>

void	clean_exit(t_cube *cube, int error_type)
{
	if (error_type == ERR_PARSING)
		printf("Error\n");
	else if (error_type == ERR_MALLOC)
		printf("Malloc error!\n");
	ft_free_gc(cube->gc);
	exit(1);
}

void	cube_init(t_cube *cube, t_parse *prs, char *map_file)
{
	int		i;

	i = -1;
	cube->gc = NULL;
	cube->map_h = 0;
	cube->map_w = 0;
	cube->player_x = -1;
	cube->player_y = -1;
	cube->lines = NULL;
	cube->prs = prs;
	cube->prs->max_x = 0;
	cube->prs->max_y = 0;
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
	t_ray	ray;
	t_keys	key;
	t_mlx	mlx;

	if (argc != 2)
		return (write(1, "Error\n", 6), 1);
	cube.mlx = &mlx;
	cube.ray = &ray;
	cube.keys = &key;
	cube_init(&cube, &prs, argv[1]);
	parse(&cube);
	render(&cube);
	ft_free_gc(cube.gc);
	return (0);
}
