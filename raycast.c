#include "cub3d.h"

void	cast_h(int rayx, float rayy, t_cube *cube)
{
	while(rayx < cube->map_h && (int)rayy < cube->map_w && rayx > 0 && rayy > 0 &&cube->map[rayx - (cube->ray_dx < 0)][(int)rayy] != '#')
	{
		if(cube->ray_dx > 0)
		{
			rayx++;
			rayy += cube->ray_slope;
		}
		else
		{
			rayx--;
			rayy -= cube->ray_slope;
		}
	}
	cube->ray_h_x = rayx;
	cube->ray_h_y = rayy;
}

void	cast_w(float rayx, int rayy, t_cube *cube)
{
	while((int)rayx < cube->map_h && rayy < cube->map_w && rayx > 0 && rayy > 0 &&cube->map[(int)rayx][rayy - (cube->ray_dy < 0)] != '#')
	{
		if(cube->ray_dy > 0)
		{
			rayy++;
			rayx += cube->ray_slope;
		}
		else
		{
			rayy--;
			rayx -= cube->ray_slope;
		}
	}
	cube->ray_w_x = rayx;
	cube->ray_w_y = rayy;
}

void	find_start_h(t_cube *cube)
{
	int		rayx;
	float	rayy;

	rayx = 0;
	rayy = 0;
	cube->ray_slope = cube->ray_dy / cube->ray_dx;
	if(cube->ray_dx == 0)
		return ;
	if(cube->ray_dx > 0)
		rayx = (int)(cube->player_x + 1);
	else
		rayx = (int)cube->player_x;
	rayy = (rayx - cube->player_x) * cube->ray_slope + cube->player_y;
	cast_h(rayx, rayy, cube);
}

void	find_start_w(t_cube *cube)
{
	int		rayy;
	float	rayx;

	rayx = 0;
	rayy = 0;
	cube->ray_slope = cube->ray_dx / cube->ray_dy;
	if(cube->ray_dy == 0)
		return ;
	if(cube->ray_dy > 0)
		rayy = (int)(cube->player_y + 1);
	else
		rayy = (int)cube->player_y;
	rayx = (rayy - cube->player_y) * cube->ray_slope + cube->player_x;
	cast_w(rayx, rayy, cube);
}

void	ray_cast(t_cube *cube)
{
	const float	delta_angle = 1.0472 / WINDOW_W;

	cube->ray_angle = cube->player_angle;
	cube->ray_angle += 0.523599;
	cube->ray_dx = cos(cube->ray_angle);
	cube->ray_dy = sin(cube->ray_angle);
	while(cube->ray_angle > cube->player_angle - 0.523599)
	{
		find_start_h(cube);
		find_start_w(cube);
		draw_nearest_ray(cube);
		cube->ray_angle -= delta_angle;
		cube->ray_dx = cos(cube->ray_angle);
		cube->ray_dy = sin(cube->ray_angle);
	}
}
