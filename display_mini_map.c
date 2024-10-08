#include "cub3d.h"
#include <math.h>
#include <stdint.h>

// void bresenham_fixed_point(t_cube *cube,  int64_t slope)
// {
//     fixed_point f;
//     fixed_point g;
//     int			x;
//     int			y;

//     f.i = (int64_t)cube->player_y*MINIMAP_SCALE << 32;
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

int check_player_position(int64_t player_x, int64_t player_y, t_cube *cube)
{
	fixed_point radius;

	radius.i = (int64_t)(cube->radius * ((int64_t)1 << 32));
	if(cube->map[(player_x + radius.i) >> 32][(player_y + radius.i) >> 32] == '#')
		return (0);
	if(cube->map[(player_x + radius.i) >> 32][(player_y - radius.i) >> 32] == '#')
		return (0);
	if(cube->map[(player_x - radius.i) >> 32][(player_y + radius.i) >> 32] == '#')
		return (0);
	if(cube->map[(player_x - radius.i) >> 32][(player_y - radius.i) >> 32] == '#')
		return (0);
	return (1);
}

void rotate_player(int keycode, t_cube *cube)
{
	if(keycode == XK_Right)
	{
		cube->player_angle -= 0.01;
		if(cube->player_angle < 0)
			cube->player_angle += 2 * PI;
		cube->player_dx.i = (int64_t)(cos(cube->player_angle) * ((int64_t)1 << 32));
		cube->player_dy.i = (int64_t)(sin(cube->player_angle) * ((int64_t)1 << 32));
	}
	else if(keycode == XK_Left)
	{
		cube->player_angle += 0.01;
		if(cube->player_angle > 2 * PI)
			cube->player_angle -= 2 * PI;
		cube->player_dx.i = (int64_t)(cos(cube->player_angle) * ((int64_t)1 << 32));
		cube->player_dy.i = (int64_t)(sin(cube->player_angle) * ((int64_t)1 << 32));
	}
	// draw_pixel(cube, (cube->ray_x.i / (float)((int64_t)1 << 32))*MINIMAP_SCALE, (cube->ray_y.i / (float)((int64_t)1 << 32))*MINIMAP_SCALE, 0xffffff);
}

void move_player(int keycode, t_cube *cube)
{
	fixed_point move_x;
	fixed_point move_y;

	move_x.i = cube->player_x.i;
	move_y.i = cube->player_y.i;
	if(keycode == XK_Up)
	{
		move_x.i += cube->player_dx.i;
		move_y.i += cube->player_dy.i;
	}
	else if(keycode == XK_Down)
	{
		move_x.i -= cube->player_dx.i;
		move_y.i -= cube->player_dy.i;
	}
	if(check_player_position(move_x.i, move_y.i, cube))
	{
		cube->player_x.i = move_x.i;
		cube->player_y.i = move_y.i;
	}
}

void draw_pixel(t_cube *cube, int x, int y, int color)
{
	int pixel_index;
	//if it's -3 it's gonna crash everything right?
	if(x < 0 || y < 0 || x > cube->map_h * MINIMAP_SCALE || y > cube->map_w * MINIMAP_SCALE)
		return ;
	pixel_index = (x * cube->mlx->size_line) + (y * (cube->mlx->pixel_bits / 8));
	cube->mlx->img_data[pixel_index] = color & 0xFF;
    cube->mlx->img_data[pixel_index + 1] = (color >> 8) & 0xFF;
    cube->mlx->img_data[pixel_index + 2] = (color >> 16) & 0xFF;
}

void draw_player(t_cube *cube, int color)
{
	int	x;
	int	y;
	float radius;

	radius = cube->radius * MINIMAP_SCALE;
	x = -MINIMAP_SCALE / 2;
	while(++x < MINIMAP_SCALE / 2)
	{
		y = -MINIMAP_SCALE / 2;
		while(++y < MINIMAP_SCALE / 2)
		{
			if(x * x + y * y <= radius * radius)
				draw_pixel(cube, ((cube->player_x.i * MINIMAP_SCALE) >> 32) + x, ((cube->player_y.i * MINIMAP_SCALE) >> 32) + y , color);
		}
	}
}

void draw_square(t_cube *cube, int x_scaled , int y_scaled, int color)
{
	int pixel_size_x;
	int pixel_size_y;

	pixel_size_x = -1;
	while(++pixel_size_x < MINIMAP_SCALE)
	{
		pixel_size_y = -1;
		while(++pixel_size_y < MINIMAP_SCALE)
		{
			if (pixel_size_x == 0 || pixel_size_y == 0)
				draw_pixel(cube, pixel_size_x + x_scaled, pixel_size_y + y_scaled, 0x000000);
			else
				draw_pixel(cube, pixel_size_x + x_scaled, pixel_size_y + y_scaled, color);
		}
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
				draw_square(cube, x * MINIMAP_SCALE, y * MINIMAP_SCALE, 0x0066b2);
			else
				draw_square(cube, x * MINIMAP_SCALE, y * MINIMAP_SCALE, 0x990000);
		}
	}
}

void	find_start_h(t_cube *cube)
{
	cube->ray_x.i = 0;
	if(cube->player_dx.i == 0)
		return ;
	if(cube->player_dx.i > 0)
		cube->ray_x.hi = cube->player_x.hi + 1;
	else
		cube->ray_x.hi = cube->player_x.hi;
	cube->ray_y.i = ((cube->ray_x.i - cube->player_x.i)  >> 16) * ((cube->player_dy.i << 16) / cube->player_dx.i) + cube->player_y.i;
	draw_pixel(cube, ((cube->ray_x.i * MINIMAP_SCALE) >> 32), ((cube->ray_y.i * MINIMAP_SCALE) >> 32), 0xff00ff);
}

void	find_start_w(t_cube *cube)
{
	cube->ray_y.i = 0;
	if(cube->player_dy.i == 0)
		return ;
	if(cube->player_dy.i > 0)
		cube->ray_y.hi = cube->player_y.hi + 1;
	else
		cube->ray_y.hi = cube->player_y.hi;
	cube->ray_x.i = ((cube->ray_y.i - cube->player_y.i) >> 16) * ((cube->player_dx.i << 16) / cube->player_dy.i) + cube->player_x.i;
}

void	raycast_h(t_cube *cube)
{
	int64_t slope;
	fixed_point ray_x = cube->ray_x;
	fixed_point ray_y = cube->ray_y;
	if (cube->player_dx.i == 0)
	    return ;
	slope = (cube->player_dy.i << 31) / (cube->player_dx.i >> 1);
	draw_pixel(cube, ((ray_x.i * MINIMAP_SCALE) >> 32), ((ray_y.i * MINIMAP_SCALE) >> 32), 0xff00ff);
	while(ray_x.hi < cube->map_h && ray_y.hi < cube->map_w && ray_x.i > 0 && ray_y.i > 0 && cube->map[ray_x.hi - (cube->player_dx.i < 0)][ray_y.hi] != '#')
	{
		if(cube->player_dx.i > 0)
		{
			ray_x.hi += 1;
			ray_y.i += slope;
		}
		else
		{
			ray_x.hi -= 1;
			ray_y.i -= slope;
		}
		draw_pixel(cube, ((ray_x.i * MINIMAP_SCALE) >> 32), ((ray_y.i * MINIMAP_SCALE) >> 32), 0xff00ff);
	}
}

void	raycast_w(t_cube *cube)
{
	int64_t slope;
	fixed_point ray_x = cube->ray_x;
	fixed_point ray_y = cube->ray_y;
	if (!cube->player_dy.i)
        return ;
	slope = (cube->player_dx.i  << 31) / (cube->player_dy.i >> 1);
	while(ray_x.hi < cube->map_h && ray_y.hi < cube->map_w && ray_x.i > 0 && ray_y.i > 0 && cube->map[ray_x.hi][ray_y.hi - (cube->player_dy.i < 0)] != '#')
	{
		if(cube->player_dy.i > 0)
		{
			ray_y.hi += 1;
			ray_x.i += slope;
		}
		else
		{
			ray_y.hi -= 1;
			ray_x.i -= slope;
		}
		draw_pixel(cube, ((ray_x.i * MINIMAP_SCALE) >> 32), ((ray_y.i * MINIMAP_SCALE) >> 32), 0xffffff);
		// printf("slope is: (%f)\n", slope);
	}
}

int put_image(t_cube *cube)
{
	draw_minimap(cube);
	draw_player(cube, 0x46eb34);
	find_start_h(cube);
	raycast_h(cube);
	draw_pixel(cube, ((cube->ray_x.i * MINIMAP_SCALE) >> 32), ((cube->ray_y.i * MINIMAP_SCALE) >> 32), 0x00ff00);
	find_start_w(cube);
	raycast_w(cube);
	draw_pixel(cube, ((cube->ray_x.i * MINIMAP_SCALE) >> 32), ((cube->ray_y.i * MINIMAP_SCALE) >> 32), 0xffffff);
	// find_star_w(cube);


	//line_algo(cube);
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
	if(keycode == XK_Escape)
		close_window(cube);
	if(keycode == XK_Up || keycode == XK_Down)
		move_player(keycode, cube);
	if(keycode == XK_Left || keycode == XK_Right)
		rotate_player(keycode, cube);
	return (0);
}



// void find_first_h(t_cube *cube)
// {
// 	if (cube->player_dx == 0)
// 		return;
// 	else if (cube->player_dx > 0)
// 	{
// 		cube->ray_x = ceil(cube->player_x) - cube->player_x;
// 		cube->ray_y = cube->ray_x * (cube->player_dy / fabs(cube->player_dx));
// 	}
// 	else
// 	{
// 		cube->ray_x = cube->player_x - floor(cube->player_x);
// 		cube->ray_y = cube->ray_x * (cube->player_dy / fabs(cube->player_dx));
// 	}
// }

// void bresenham_fixed_point(t_cube *cube)
// {
//     fixed_point f;
//     int x;
// 	int64_t m;

// 	m = (cube->player_dx != 0) ? cube->player_dy / cube->player_dx : 0; // Ensure dx is not zero before division

//     // Initialize f.i with y1 in fixed-point format
//     f.i = (int64_t)cube->ray_x << 32;

//     // Iterate through x coordinates from x1 to x2
//     for (x = x1; x <= x2; x++, f.i += m) {
//         fixed_point g = f;
//         g.i += 32767; // Add rounding adjustment

//         // Check if the grid at (x, g.hi >> 32) is blocking
//         if (grid[g.hi][x] == BLOCKING) {
//             return;
//         }
//     }
// }


// void line_algo(t_cube * cube)
// {
// 	float    slope;
//     float    start_x;
//     float    start_y;

// 	if (cube->player_dx == 0)
// 	{
// 		start_x = cube->player_x;
// 		start_y = (int)(cube->player_y + (cube->player_dy > 0));
// 	}
// 	else if (cube->player_dy == 0)
// 	{
// 		start_x = (int)(cube->player_x + (cube->player_dx > 0));
// 		start_y = cube->player_y;
// 	}
// 	else
// 	{
//     	slope = cube->player_dy / cube->player_dx;
//     	if (fabs(((int)(cube->player_y + (cube->player_dy > 0)) - cube->player_y) / ((int)(cube->player_x + (cube->player_dx > 0)) - cube->player_x)) > fabs(slope))
//     	{
//         start_x = (int)(cube->player_x + (cube->player_dx > 0));
//         start_y = cube->player_y + (start_x - cube->player_x) * slope;
// 		printf("player y: %f\n", cube->player_y);
// 		printf("dif in x: %f\n", ((int)(cube->player_x + (cube->player_dx > 0)) - cube->player_x));
// 		printf("slope : %f\n", slope);
//     	}
//     	else if (fabs(((int)(cube->player_y + (cube->player_dy > 0)) - cube->player_y) / ((int)(cube->player_x + (cube->player_dx > 0)) - cube->player_x)) < fabs(slope))
// 		{
// 		start_y = (int)(cube->player_y + (cube->player_dy > 0));
// 		start_x = cube->player_x + (start_y - cube->player_y) / slope;
// 		}
// 		else
// 		{
// 			start_x = cube->player_x + (cube->player_dx > 0);
// 			start_y = cube->player_y + (cube->player_dy > 0);
// 		}
// 	}
//     draw_pixel(cube, start_x*MINIMAP_SCALE, start_y*MINIMAP_SCALE, 0xFFFFFF);
// }

// void find_wall(t_cube *cube, int cube->player_dx, int dir_y)
// {
// 	int pd = sqrt(pow(cube->player_x - cube->player_dx, 2) + pow(cube->player_y - dir_y, 2));
// }

void render(t_cube *cube)
{
	copy_player_map(cube);
	for (int x = 0; x < cube->map_h; x++)
	{
	 	for (int y = 0; y < cube->map_w; y++)
			printf("%c", cube->map[x][y]);
		printf("\n");
	}
	cube->mlx->mlx_ptr = mlx_init();
	cube->mlx->win_ptr = mlx_new_window(cube->mlx->mlx_ptr, WINDOW_W, WINDOW_H, "diplsay mini_map");
	cube->mlx->image = mlx_new_image(cube->mlx->mlx_ptr, cube->map_w*MINIMAP_SCALE, cube->map_h*MINIMAP_SCALE);
	cube->mlx->img_data = mlx_get_data_addr(cube->mlx->image, &cube->mlx->pixel_bits, &cube->mlx->size_line, &cube->mlx->endian);
	cube->player_x.i += (int64_t)(0.5 * ((int64_t)1 << 32));
	cube->player_y.i += (int64_t)(0.5 * ((int64_t)1 << 32));
	cube->ray_x.i = 0;
	cube->ray_y.i = 0;
	cube->ray_x.hi = 0;
	cube->ray_y.hi = 0;
	cube->radius = 0.5;
	mlx_hook(cube->mlx->win_ptr, 17, 0, close_window, cube);
	mlx_hook(cube->mlx->win_ptr, KeyPress, KeyPressMask, key_handler, cube);
	mlx_loop_hook(cube->mlx->mlx_ptr, put_image, cube);
	mlx_loop(cube->mlx->mlx_ptr);
}
