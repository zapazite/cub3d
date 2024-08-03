#include "cub3d.h"
#include "minilibx-linux/mlx.h"
#include <X11/X.h>
#include <X11/Xutil.h>
#include <math.h>

void display_square(void *mlx, void *winptr, int i , int j, int pixel_size, int color)
{
	int x = pixel_size;
	int y = pixel_size;
	while(x--)
	{
		y = pixel_size;
		while(y--)
			mlx_pixel_put(mlx, winptr, x + i, y + j, color);
	}
}

void display_player(void *mlx, void *winptr, int i , int j, int pixel_size, int color)
{
	int x = pixel_size;
	int y = pixel_size;
	float radius = pixel_size / 2.0;
	while(x--)
	{
		y = pixel_size;
		while(y--)
		{
			if(pow(radius - x, 2) + pow(radius - y, 2) <= pow(radius, 2))
				mlx_pixel_put(mlx, winptr, x + i, y + j, color);
		}
	}
}

int move_player(int keycode, t_cube *cube)
{

	if(keycode == XK_Left) //left arrow
		cube->spawn_y -= 0.2;
	else if(keycode == XK_Right) //right arrow
		cube->spawn_y += 0.2;
	else if(keycode == XK_Up) //up arrow
		cube->spawn_x -= 0.2;
	else if(keycode == XK_Down) //down arrow
		cube->spawn_x += 0.2;
	return (0);
}

int render(t_cube *cube)
{
	int i = cube->map_h;
	int scale = 50;
	while(i--)
	{
		int j = cube->map_w;
		while (j--)
		{
			if(cube->map[i][j] == '1')
				display_square(cube->mlx_ptr, cube->win_ptr, j*scale, i*scale, scale, 0x0066b2);
			else if(cube->map[i][j] == '0' || cube->map[i][j] == '!')
				display_square(cube->mlx_ptr, cube->win_ptr, j*scale, i*scale, scale, 0x00d43c);
		}
	}
	display_player(cube->mlx_ptr, cube->win_ptr, cube->spawn_y*scale, cube->spawn_x*scale, scale, 0x990000);
	return (0);
}

void display_mini_map(t_cube *cube)
{
	cube->mlx_ptr = mlx_init();
	cube->win_ptr = mlx_new_window(cube->mlx_ptr, 1920, 1080, "diplsay mini_map");
	mlx_hook(cube->win_ptr, KeyPress,KeyPressMask, move_player, cube);
	mlx_loop_hook(cube->mlx_ptr, render, cube);
	mlx_loop(cube->mlx_ptr);
}
