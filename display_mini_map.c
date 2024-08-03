#include "cub3d.h"

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

void draw_pixel(t_cube *cube, int x, int y, int color)
{
	int pixel_index = (x * cube->size_line) + (y * (cube->pixel_bits / 8));
	cube->img_data[pixel_index] = color & 0xFF;
    cube->img_data[pixel_index + 1] = (color >> 8) & 0xFF;
    cube->img_data[pixel_index + 2] = (color >> 16) & 0xFF;
}

void draw_player(t_cube *cube, int i , int j, int pixel_size, int color)
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
				draw_pixel(cube, x + i, y + j, color);
		}
	}
}

void draw_square(t_cube *cube, int i , int j, int pixel_size, int color)
{
	int x = pixel_size;
	int y = pixel_size;

	while(x--)
	{
		y = pixel_size;
		while(y--)
			draw_pixel(cube, x + i, y + j, color);
	}
}

void draw_minimap(t_cube *cube)
{
	int i;
	int j;

	i = cube->max_x + 1;
	while(i-- > cube->min_x)
	{
		j = cube->max_y + 1;
		while(j-- > cube->min_y)
		{
			if(cube->map[i][j] == '!')
				draw_square(cube, (i - cube->min_x)*MINIMAP_SCALE, (j - cube->min_y)*MINIMAP_SCALE, MINIMAP_SCALE, 0x0066b2);
			else if(cube->map[i][j] == '#')
				draw_square(cube, (i - cube->min_x)*MINIMAP_SCALE, (j - cube->min_y)*MINIMAP_SCALE, MINIMAP_SCALE, 0x990000);
		}
	}
}

int put_image(t_cube *cube)
{
	draw_minimap(cube);
	draw_player(cube, cube->spawn_x*MINIMAP_SCALE, cube->spawn_y*MINIMAP_SCALE, cube->pixel_bits, 0x46eb34);
	printf("pos x %f\n", cube->spawn_x);
	printf("pos y %f\n", cube->spawn_y);

	mlx_put_image_to_window(cube->mlx_ptr, cube->win_ptr, cube->image, 0, 0);
	return 0;
}

void display_mini_map(t_cube *cube)
{
	cube->mlx_ptr = mlx_init();
	cube->win_ptr = mlx_new_window(cube->mlx_ptr, 1920, 1080, "diplsay mini_map");
	cube->image = mlx_new_image(cube->mlx_ptr, (cube->max_y - cube->min_y + 1)*MINIMAP_SCALE, (cube->max_x - cube->min_x + 1)*MINIMAP_SCALE);
	cube->img_data = mlx_get_data_addr(cube->image, &cube->pixel_bits, &cube->size_line, &cube->endian);
	mlx_hook(cube->win_ptr, KeyPress,KeyPressMask, move_player, cube);
	mlx_loop_hook(cube->mlx_ptr, put_image, cube);
	mlx_loop(cube->mlx_ptr);
}
