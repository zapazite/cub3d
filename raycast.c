#include "cub3d.h"
#include "gc/gc.h"
#include "minilibx-linux/mlx.h"
#include <math.h>

void	cast_h(int rayx, float rayy, t_cube *cube)
{
	while(rayx < cube->map_h && (int)rayy < cube->map_w && rayx > 0 && rayy > 0 &&cube->map[rayx - (cube->ray->dx < 0)][(int)rayy] != '#')
	{
		if(cube->ray->dx > 0)
		{
			rayx++;
			rayy += cube->ray->slope;
		}
		else
		{
			rayx--;
			rayy -= cube->ray->slope;
		}
	}
	cube->ray->h_x = rayx;
	cube->ray->h_y = rayy;
}

void	cast_w(float rayx, int rayy, t_cube *cube)
{
	while((int)rayx < cube->map_h && rayy < cube->map_w && rayx > 0 && rayy > 0 &&cube->map[(int)rayx][rayy - (cube->ray->dy < 0)] != '#')
	{
		if(cube->ray->dy > 0)
		{
			rayy++;
			rayx += cube->ray->slope;
		}
		else
		{
			rayy--;
			rayx -= cube->ray->slope;
		}
	}
	cube->ray->w_x = rayx;
	cube->ray->w_y = rayy;
}

void	find_start_h(t_cube *cube)
{
	int		rayx;
	float	rayy;

	rayx = 0;
	rayy = 0;
	cube->ray->slope = cube->ray->dy / cube->ray->dx;
	if(cube->ray->dx == 0)
		return ;
	if(cube->ray->dx > 0)
		rayx = (int)(cube->player_x + 1);
	else
		rayx = (int)cube->player_x;
	rayy = (rayx - cube->player_x) * cube->ray->slope + cube->player_y;
	cast_h(rayx, rayy, cube);
}

void	find_start_w(t_cube *cube)
{
	int		rayy;
	float	rayx;

	rayx = 0;
	rayy = 0;
	cube->ray->slope = cube->ray->dx / cube->ray->dy;
	if(cube->ray->dy == 0)
		return ;
	if(cube->ray->dy > 0)
		rayy = (int)(cube->player_y + 1);
	else
		rayy = (int)cube->player_y;
	rayx = (rayy - cube->player_y) * cube->ray->slope + cube->player_x;
	cast_w(rayx, rayy, cube);
}

void	draw_world(t_cube *cube)
{
	int		i;
	int		j;
	float	wall_size;

	i = -1;
	while(++i < WINDOW_H)
	{
		j = -1;
		while(++j < WINDOW_W)
		{
			wall_size = WINDOW_W * sqrt(3) / cube->ray->hit_points[j] / 2;
			if(i < WINDOW_H / 2. - (wall_size / 2))
				draw_main_pixel(cube, i, j, cube->colors[CIELLING]);
			else if (i > WINDOW_H / 2. + (wall_size / 2))
				draw_main_pixel(cube, i, j, cube->colors[FLOOR]);
			else
				draw_main_pixel(cube, i, j, 0xbc001a);
		}
	}
}

void	ray_cast(t_cube *cube)
{
	int		i;
	float	d_player_screen;

	i = 0;
	cube->ray->dx = (WINDOW_W / 2.) / tan(FOV / 2) * cube->player_dx;
	cube->ray->dy = (WINDOW_W / 2.) / tan(FOV / 2) * cube->player_dy;
	d_player_screen = sqrt(pow(cube->ray->dx, 2) + pow(cube->ray->dy, 2));
	cube->ray->dx += WINDOW_W / 2. * -cube->player_dy;
	cube->ray->dy += WINDOW_W / 2. * cube->player_dx;
	while(i < WINDOW_W)
	{
		find_start_h(cube);
		find_start_w(cube);
		save_ray_distance(cube, i, d_player_screen);
		cube->ray->dx -= -cube->player_dy;
		cube->ray->dy -= cube->player_dx;
		i++;
	}
}
