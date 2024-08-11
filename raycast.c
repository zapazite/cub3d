#include "cub3d.h"
#include "gc/gc.h"
#include "minilibx-linux/mlx.h"

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

void	draw_walls(t_cube *cube, float hit_distance)
{
	float wall_h;
	int i;

	wall_h = (50*WINDOW_H) / hit_distance;
	while(wall_h > 0)
	{
		draw_main_pixel(cube, (cube->ray->hit_points[i][0]*50)+wall_h, cube->ray->hit_points[i][1]*50, 0xff0000);
		wall_h--;
	}
}

void	ray_cast(t_cube *cube)
{
	int		i;
	float	hit_distance;

	i = 0;
	cube->ray->angle = cube->player_angle;
	cube->ray->angle += 0.523599; //30 degrees offset
	cube->ray->dx = cos(cube->ray->angle);
	cube->ray->dy = sin(cube->ray->angle);
	while(cube->ray->angle > cube->player_angle - 0.523599)
	{
		find_start_h(cube);
		find_start_w(cube);
		hit_distance = draw_nearest_ray(cube, i);
		cube->ray->angle -= cube->ray->d_angle;
		cube->ray->dx = cos(cube->ray->angle);
		cube->ray->dy = sin(cube->ray->angle);
		i++;
	}
	draw_walls(cube, hit_distance);
}
