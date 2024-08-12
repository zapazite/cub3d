#include "cub3d.h"

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

void	save_ray_distance(t_cube *cube, int i, float d_player_screen)
{
	float h_hit;
	float w_hit;

	h_hit = pow(cube->player_x - cube->ray->h_x, 2) + pow(cube->player_y - cube->ray->h_y, 2);
	w_hit = pow(cube->player_x - cube->ray->w_x, 2) + pow(cube->player_y - cube->ray->w_y, 2);
	if(h_hit <= w_hit)
		cube->ray->hit_points[i] = sqrt(h_hit) * d_player_screen / (sqrt(pow(cube->ray->dx, 2) + pow(cube->ray->dy, 2)));
	else
		cube->ray->hit_points[i] = sqrt(w_hit) * d_player_screen / (sqrt(pow(cube->ray->dx, 2) + pow(cube->ray->dy, 2)));
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
