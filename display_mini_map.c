#include "cub3d.h"
#include "gc/gc.h"
#include "minilibx-linux/mlx.h"
#include <X11/Xutil.h>
#include <math.h>
#include <stdint.h>
#include <unistd.h>

// void bresenham_fixed_point(t_cube *cube,  int32_t slope)
// {
//     fixed_point f;
//     fixed_point g;
//     int			x;
//     int			y;

//     f.i = (int32_t)cube->player_y*MINIMAP_SCALE << 16;
//     // Iterate through x coordinates from x1 to x2
//     x = cube->player_x*MINIMAP_SCALE;
//     y = cube->player_y*MINIMAP_SCALE;
//     while(cube->map[(x / MINIMAP_SCALE)][(y / MINIMAP_SCALE)] != '#')
//     {
//     	draw_pixel(cube, x, y, 0xFFFFFF);
//     }
//     // for (x = x1; x <= x2; x++, f.i += m) {
//     //     fixed_point g = f;
//     //     g.i += 32767; // Add rounding adjustment

//     //     // Check if the grid at (x, g.hi >> 16) is blocking
//     //     if (grid[g.hi][x] == "#")
//     //     {
//     //         return;
//     //     }
//     // }
// }

int check_player_position(float player_x, float player_y, t_cube *cube)
{
	if(cube->map[(int)((player_x + cube->radius))][(int)((player_y + cube->radius))] == '#')
		return (0);
	if(cube->map[(int)((player_x + cube->radius))][(int)((player_y - cube->radius))] == '#')
		return (0);
	if(cube->map[(int)((player_x - cube->radius))][(int)((player_y + cube->radius))] == '#')
		return (0);
	if(cube->map[(int)((player_x - cube->radius))][(int)((player_y - cube->radius))] == '#')
		return (0);
	return (1);
}

void move_player(int keycode, t_cube *cube)
{
	float move_x;
	float move_y;

	//need to move player closer to the wall
	move_x = cube->player_x;
	move_y = cube->player_y;
	if(keycode == XK_Left) //left arrow
		move_y -= 0.1;
	else if(keycode == XK_Right) //right arrow
		move_y += 0.1;
	else if(keycode == XK_Up) //up arrow
		move_x -= 0.1;
	else if(keycode == XK_Down) //down arrow
		move_x += 0.1;
	if(check_player_position(move_x, move_y, cube))
	{
		cube->player_x = move_x;
		cube->player_y = move_y;
	}
}


void draw_pixel(t_cube *cube, int x, int y, int color)
{
	int pixel_index;

	pixel_index = (x * cube->mlx->size_line) + (y * (cube->mlx->pixel_bits / 8));
	cube->mlx->img_data[pixel_index] = color & 0xFF;
    cube->mlx->img_data[pixel_index + 1] = (color >> 8) & 0xFF;
    cube->mlx->img_data[pixel_index + 2] = (color >> 16) & 0xFF;
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
			if(pow(cube->radius*MINIMAP_SCALE - x, 2) + pow(cube->radius*MINIMAP_SCALE - y, 2) <= pow(cube->radius*MINIMAP_SCALE, 2))
				draw_pixel(cube, x + (cube->player_x - cube->radius)*MINIMAP_SCALE, y + (cube->player_y - cube->radius)*MINIMAP_SCALE, color);
	}
	draw_pixel(cube, (cube->player_x)*MINIMAP_SCALE, (cube->player_y)*MINIMAP_SCALE, 0xFF0000);
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
	line_algo(cube, 1, -21);
	mlx_put_image_to_window(cube->mlx->mlx_ptr, cube->mlx->win_ptr, cube->mlx->image, 0, 0);
	return 0;
}

void copy_player_map(t_cube *cube)
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

int close_window(t_cube *cube)
{
	ft_free_gc(cube->gc);
	mlx_destroy_image(cube->mlx->mlx_ptr, cube->mlx->image);
	mlx_destroy_window(cube->mlx->mlx_ptr, cube->mlx->win_ptr);
	mlx_destroy_display(cube->mlx->mlx_ptr);
	free(cube->mlx->mlx_ptr);
	exit(0);
}

int key_handler(int keycode, t_cube *cube)
{
	move_player(keycode, cube);
	if(keycode == XK_Escape)
		close_window(cube);
	return (0);
}

// void line_algo(t_cube * cube, int dir_x, int dir_y)
// {
// 	float    slope;
//     float    start_x;
//     float    start_y;

//     slope = (float)dir_y / dir_x;

//     if ((((int)(cube->player_y + (dir_y > 0)) - cube->player_y) / ((int)(cube->player_x + (dir_x > 0)) - cube->player_x)) > slope)
//     {
//         start_x = (int)(cube->player_x + (dir_x > 0));
//         start_y = cube->player_y + ((int)(cube->player_x + (dir_x > 0)) - cube->player_x) * slope;
//     }
//     else
//     	return;
//     printf("start x: %f\n", start_x);
//     printf("start y: %f\n", start_y);
//     printf("player x: %f\n", cube->player_x);
//     printf("player y: %f\n", cube->player_y);
//     draw_pixel(cube, start_x*MINIMAP_SCALE, start_y*MINIMAP_SCALE, 0xFFFFFF);
// }

void find_wall(t_cube *cube, int dir_x, int dir_y)
{
	int pd = sqrt(pow(cube->player_x - dir_x, 2) + pow(cube->player_y - dir_y, 2))
}

void render(t_cube *cube)
{
	copy_player_map(cube);
	for (int i = 0; i < cube->map_h; i++)
	{
	 	for (int j = 0; j < cube->map_w; j++)
			printf("%c", cube->map[i][j]);
		printf("\n");
	}
	cube->mlx->mlx_ptr = mlx_init();
	cube->mlx->win_ptr = mlx_new_window(cube->mlx->mlx_ptr, WINDOW_W, WINDOW_H, "diplsay mini_map");
	cube->mlx->image = mlx_new_image(cube->mlx->mlx_ptr, cube->map_w*MINIMAP_SCALE, cube->map_h*MINIMAP_SCALE);
	cube->mlx->img_data = mlx_get_data_addr(cube->mlx->image, &cube->mlx->pixel_bits, &cube->mlx->size_line, &cube->mlx->endian);
	cube->player_x += 0.5;
	cube->player_y += 0.5;
	cube->radius = 0.2;

	mlx_hook(cube->mlx->win_ptr, 17, 0, close_window, cube);
	mlx_hook(cube->mlx->win_ptr, KeyPress,KeyPressMask, key_handler, cube);
	mlx_loop_hook(cube->mlx->mlx_ptr, put_image, cube);
	mlx_loop(cube->mlx->mlx_ptr);
}
