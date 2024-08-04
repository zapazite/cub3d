#include "cub3d.h"

// int check_player_position(t_cube *cube)
// {
// 	//what are coordinates of the walls?
// 	//check that the position of the centre of the player is not bigger than those coordinate
// 	if(cube->spawn_x < cube->min_x)
// 		return (1);
// 	// if(cube->spawn_y <= cube->min_y || cube->spawn_y >= cube->max_y)
// 	// 	return (1);
// 	return (0);
// }

int move_player(int keycode, t_cube *cube)
{
	// if(check_player_position(cube))
	// 	return 0;
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

void draw_player(t_cube *cube, int color)
{
	int		x;
	int		y;

	x = cube->pixel_bits;
	y = cube->pixel_bits;
	cube->radius = cube->pixel_bits / 2.0;
	while(x--)
	{
		y = cube->pixel_bits;
		while(y--)
			if(pow(cube->radius - x, 2) + pow(cube->radius - y, 2) <= pow(cube->radius, 2))
				draw_pixel(cube, x + cube->spawn_x*MINIMAP_SCALE, y + cube->spawn_y*MINIMAP_SCALE, color);
	}
}

void draw_square(t_cube *cube, int x_scaled , int y_scaled, int color)
{
	int pixel_size_x;
	int pixel_size_y;

	pixel_size_x = MINIMAP_SCALE;
	pixel_size_y = MINIMAP_SCALE;
	while(pixel_size_x--)
	{
		pixel_size_y = MINIMAP_SCALE;
		while(pixel_size_y--)
			draw_pixel(cube, pixel_size_x + x_scaled, pixel_size_y + y_scaled, color);
	}
}

void draw_minimap(t_cube *cube)
{
	int x;
	int y;

	x = cube->max_x + 1;
	while(x-- > cube->min_x)
	{
		y = cube->max_y + 1;
		while(y-- > cube->min_y)
		{
			if(cube->map[x][y] == '!')
				draw_square(cube, (x - cube->min_x)*MINIMAP_SCALE, (y - cube->min_y)*MINIMAP_SCALE, 0x0066b2);
			else if(cube->map[x][y] == '#')
				draw_square(cube, (x - cube->min_x)*MINIMAP_SCALE, (y - cube->min_y)*MINIMAP_SCALE, 0x990000);
		}
	}
}

int put_image(t_cube *cube)
{
	draw_minimap(cube);
	// if(check_player_position(cube) == 0)
		draw_player(cube, 0x46eb34);
	mlx_put_image_to_window(cube->mlx_ptr, cube->win_ptr, cube->image, 0, 0);
	return 0;
}

void display_mini_map(t_cube *cube)
{
	cube->mlx_ptr = mlx_init();
	cube->win_ptr = mlx_new_window(cube->mlx_ptr, WINDOW_W, WINDOW_H, "diplsay mini_map");
	cube->image = mlx_new_image(cube->mlx_ptr, (cube->max_y - cube->min_y + 1)*MINIMAP_SCALE, (cube->max_x - cube->min_x + 1)*MINIMAP_SCALE);
	cube->img_data = mlx_get_data_addr(cube->image, &cube->pixel_bits, &cube->size_line, &cube->endian);
	mlx_hook(cube->win_ptr, KeyPress,KeyPressMask, move_player, cube);
	mlx_loop_hook(cube->mlx_ptr, put_image, cube);
	mlx_loop(cube->mlx_ptr);
}
