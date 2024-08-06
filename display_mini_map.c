#include "cub3d.h"
#include <unistd.h>

int check_player_position(float player_x, float player_y, t_cube *cube)
{
	//this code needs to be rewritten
	if(cube->map[(int)((player_x + (cube->radius / (MINIMAP_SCALE))))][(int)((player_y + (cube->radius / (MINIMAP_SCALE))))] == '#')
		return (0);
	if(cube->map[(int)((player_x - (cube->radius / (MINIMAP_SCALE))))][(int)((player_y + (cube->radius / (MINIMAP_SCALE))))] == '#')
		return (0);
	if(cube->map[(int)((player_x + (cube->radius / (MINIMAP_SCALE))))][(int)((player_y - (cube->radius / (MINIMAP_SCALE))))] == '#')
		return (0);
	if(cube->map[(int)((player_x - (cube->radius / (MINIMAP_SCALE))))][(int)((player_y - (cube->radius / (MINIMAP_SCALE))))] == '#')
		return (0);
	return (1);
}

int move_player(int keycode, t_cube *cube)
{
	float move_x;
	float move_y;

	move_x = cube->spawn_x;
	move_y = cube->spawn_y;
	//if it goes over the wall just move player to against the wall instead of resetting
	printf("x %f\n", cube->spawn_x);
	printf("y %f\n", cube->spawn_y);
	if(keycode == XK_Left) //left arrow
		move_y -= 0.3;
	else if(keycode == XK_Right) //right arrow
		move_y += 0.3;
	else if(keycode == XK_Up) //up arrow
		move_x -= 0.3;
	else if(keycode == XK_Down) //down arrow
		move_x += 0.3;
	if(check_player_position(move_x, move_y, cube))
	{
		cube->spawn_x = move_x;
		cube->spawn_y = move_y;
	}
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

	x = MINIMAP_SCALE;
	while(x--)
	{
		y = MINIMAP_SCALE;
		while(y--)
			if(pow(cube->radius - x, 2) + pow(cube->radius - y, 2) <= pow(cube->radius, 2))
				draw_pixel(cube, x + (cube->spawn_x - 0.5)*MINIMAP_SCALE, y + (cube->spawn_y - 0.5)*MINIMAP_SCALE, color);
	}
	draw_pixel(cube, (cube->spawn_x)*MINIMAP_SCALE, (cube->spawn_y)*MINIMAP_SCALE, 0xFF0000);
}

void draw_square(t_cube *cube, int x_scaled , int y_scaled, int color)
{
	//scale so that 1 pixel now is MINIMAP_SCALE*MINIMAP_SCALE
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
	int x = 0;
	int y = 0;

	x = cube->map_h;
	while(x-- > 0)
	{
		y = cube->map_w;
		while(y-- > 0)
		{
			if(cube->map[x][y] == '!')
				draw_square(cube, x*MINIMAP_SCALE, y*MINIMAP_SCALE, 0x0066b2);
			else if(cube->map[x][y] == '#')
				draw_square(cube, x*MINIMAP_SCALE, y*MINIMAP_SCALE, 0x990000);
		}
	}
}

int put_image(t_cube *cube)
{
	draw_minimap(cube);
	draw_player(cube, 0x46eb34);
	mlx_put_image_to_window(cube->mlx_ptr, cube->win_ptr, cube->image, 0, 0);
	return 0;
}

void copy_player_map(t_cube *cube)
{
	int x;
	int	y;

	printf("min x %d\n", cube->prs->min_x);fflush(stdout);
	printf("min y %d\n", cube->prs->min_y);fflush(stdout);
	cube->map_w = (cube->prs->max_x - cube->prs->min_x) + 1;
	cube->map_h = (cube->prs->max_y - cube->prs->min_y) + 1;

	cube->map = (int **)ft_malloc(cube, sizeof(int *) *cube->map_h);
	x = -1;
	while(++x < cube->map_h)
	{
		cube->map[x] = (int *)ft_malloc(cube, sizeof(int) * cube->map_w);
		y = -1;
		while(++y < cube->map_w)
			cube->map[x][y] = cube->prs->prs_map[cube->prs->min_x + x][cube->prs->min_y + y];
	}
}

void display_mini_map(t_cube *cube)
{
	copy_player_map(cube); //copy old map into new small one;
	// for (int i = 0; i < cube->n_map_size_x; i++)
	// {
	//  	for (int j = 0; j < cube->n_map_size_y; j++)
	// 		printf("%c", cube->map[i][j]);
	// 	printf("\n");
	// }
	cube->spawn_x += 0.5;
	cube->spawn_y += 0.5;
	cube->mlx_ptr = mlx_init();
	cube->win_ptr = mlx_new_window(cube->mlx_ptr, WINDOW_W, WINDOW_H, "diplsay mini_map");
	cube->image = mlx_new_image(cube->mlx_ptr, cube->map_w*MINIMAP_SCALE, cube->map_h*MINIMAP_SCALE);
	cube->img_data = mlx_get_data_addr(cube->image, &cube->pixel_bits, &cube->size_line, &cube->endian);
	cube->radius = MINIMAP_SCALE / 2.0; //player size
	mlx_hook(cube->win_ptr, KeyPress,KeyPressMask, move_player, cube);
	mlx_loop_hook(cube->mlx_ptr, put_image, cube);
	mlx_loop(cube->mlx_ptr);
}
