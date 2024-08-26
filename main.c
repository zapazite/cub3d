/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efaiz <efaiz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 10:35:49 by mde-prin          #+#    #+#             */
/*   Updated: 2024/08/23 15:30:50 by efaiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	clean_exit(t_cube *cube, int error_type)
{
	if (error_type == ERR_PARSING)
		{
			printf("Error\n");
			printf("Invalid Map!\n");
		}
	else if (error_type == ERR_MALLOC)
		printf("Malloc error!\n");
	ft_free_gc(cube->gc);
	exit(1);
}

void	cube_init(t_cube *cube, t_parse *prs, char *map_file)
{
	int	i;

	cube->gc = NULL;
	cube->map_h = 0;
	cube->map_w = 0;
	cube->anim->counter = 0;
	cube->player_x = -1;
	cube->player_y = -1;
	cube->lines = NULL;
	cube->prs = prs;
	cube->ray->door_check_flag = 0;
	cube->prs->max_x = 0;
	cube->prs->max_y = 0;
	cube->prs->map_file = map_file;
	i = -1;
	while (++i < 2)
		cube->colors[i] = -1;
	i = -1;
	while (++i < 4)
		cube->paths[i][0] = '\0';
	cube->draw->wall_idx = 0;
	cube->draw->wall_size = 0;
	cube->draw->wall_offset = 0;
	cube->draw->txt = 0;
}

void	main_2(t_cube *cube, char *argv)
{
	t_mlx		mlx;
	t_textures	textures;
	t_anim		anim;
	t_draw		draw;

	cube->txt = &textures;
	cube->anim = &anim;
	cube->draw = &draw;
	cube->mlx = &mlx;
	cube_init(cube, cube->prs, argv);
	parse(cube);
	render(cube);
}

int	main(int argc, char *argv[])
{
	t_cube		cube;
	t_parse		prs;
	t_ray		ray;
	t_keys		key;

	if (argc != 2)
		return (write(1, "Error\n", 6), 1);
	cube.ray = &ray;
	cube.keys = &key;
	cube.prs = &prs;
	main_2(&cube, argv[1]);
	ft_free_gc(cube.gc);
	return (0);
}
