#include "cub3d.h"

void copy_playable_map(t_cube *cube)
{
	int x;
	int	y;

	cube->map_h = (cube->prs->max_x - cube->prs->min_x) + 1;
	cube->map_w = (cube->prs->max_y - cube->prs->min_y) + 1;
	cube->map = (int **)ft_malloc(cube, sizeof(int *) *cube->map_h);
	x = -1;
	while(++x < cube->map_h)
	{
		cube->map[x] = (int *)ft_malloc(cube, sizeof(int) * cube->map_w);
		y = -1;
		while(++y < cube->map_w)
		{
			cube->map[x][y] = cube->prs->prs_map[cube->prs->min_x + x][cube->prs->min_y + y];
			if(cube->map[x][y] == '1')
				cube->map[x][y] = '#';
		}
	}
}

void init_player(t_cube *cube)
{
	cube->player_x += 0.5;
	cube->player_y += 0.5;
	cube->radius = 0.3;
	cube->player_angle = 0;
	cube->player_dx = cos(cube->player_angle);
	cube->player_dy = sin(cube->player_angle);
}

void init_ray(t_cube *cube)
{
	cube->ray_angle = cube->player_angle;
	cube->ray_angle += 0.523599;
	cube->ray_dx = cos(cube->ray_angle);
	cube->ray_dy = sin(cube->ray_angle);
}

int close_window(t_cube *cube)
{
	ft_free_gc(cube->gc);
	mlx_destroy_image(cube->mlx->mlx_ptr, cube->mlx->image);
	mlx_destroy_window(cube->mlx->mlx_ptr, cube->mlx->win_ptr);
	mlx_destroy_display(cube->mlx->mlx_ptr);
	free(cube->mlx->mlx_ptr);
	exit(0);
}
