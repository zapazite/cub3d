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
	cube->player_jump = 0;
	cube->player_y += 0.5;
	cube->radius = 0.3;
	cube->player_dx = cos(cube->player_angle);
	cube->player_dy = sin(cube->player_angle);
}

void	init_keyes(t_cube *cube)
{
	cube->keys->key_w = 0;
	cube->keys->key_s = 0;
	cube->keys->key_d = 0;
	cube->keys->key_a = 0;
	cube->keys->key_up = 0;
	cube->keys->key_down = 0;
	cube->keys->key_left = 0;
	cube->keys->key_right = 0;
}

int close_window(t_cube *cube)
{
	ft_free_gc(cube->gc);
	mlx_destroy_image(cube->mlx->mlx_ptr, cube->mlx->map_img);
	mlx_destroy_image(cube->mlx->mlx_ptr, cube->mlx->main_img);
	mlx_destroy_window(cube->mlx->mlx_ptr, cube->mlx->win_ptr);
	mlx_destroy_display(cube->mlx->mlx_ptr);
	free(cube->mlx->mlx_ptr);
	exit(0);
}

int	door_check(t_cube *cube, float x, float y, char line)
{
	float	line_percent;
	float	door_percent;

	if(line == 'h')
		line_percent = ((y - (int)y) / ((int)y + 1 - (int)y)) * 100;
	else
		line_percent = ((x - (int)x) / ((int)x + 1 - (int)x)) * 100;
	if(cube->map[(int)x][(int)y] >= 1000 && cube->map[(int)x][(int)y] <= 2000)
	{
		door_percent = fabs(((cube->map[(int)x][(int)y] - 2000.) / 1000)) * 100;
		if(line_percent <= door_percent)
		{
			cube->ray->door_check_flag = door_percent;
			return (1);
		}
	}
	cube->ray->door_check_flag = 0;
	return (0);
}
