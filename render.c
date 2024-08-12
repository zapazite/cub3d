#include "cub3d.h"
#include "minilibx-linux/mlx.h"
#include <stdio.h>

int	key_handler(int keycode, t_cube *cube)
{
	if(keycode == XK_Escape)
		close_window(cube);
	if(keycode == XK_Up || keycode == XK_w)
	{
		cube->keys->key_w = 1;
		cube->keys->key_up = 1;
	}
	if(keycode == XK_Down || keycode == XK_s )
	{
		cube->keys->key_down = 1;
		cube->keys->key_s = 1;
	}
	if(keycode == XK_Left || keycode == XK_a)
	{
		cube->keys->key_a = 1;
		cube->keys->key_left = 1;
	}
	if(keycode == XK_Right || keycode == XK_d)
	{
		cube->keys->key_d = 1;
		cube->keys->key_right = 1;
	}
	return (0);
}

int key_release(int keycode, t_cube *cube)
{
	if(keycode == XK_Up || keycode == XK_w)
	{
		cube->keys->key_w = 0;
		cube->keys->key_up = 0;
	}
	if(keycode == XK_Down || keycode == XK_s)
	{
		cube->keys->key_s = 0;
		cube->keys->key_down = 0;
	}
	if(keycode == XK_Left || keycode == XK_a)
	{
		cube->keys->key_a = 0;
		cube->keys->key_left = 0;
	}
	if(keycode == XK_Right || keycode == XK_d)
	{
		cube->keys->key_d = 0;
		cube->keys->key_right = 0;
	}
	return (0);
}

int put_image(t_cube *cube)
{
	move_player(cube);
	rotate_player(cube);
	draw_minimap(cube);
	draw_player(cube);
	ray_cast(cube);
	draw_world(cube);
	mlx_put_image_to_window(cube->mlx->mlx_ptr, cube->mlx->win_ptr, cube->mlx->main_img, 0, 0);
	mlx_put_image_to_window(cube->mlx->mlx_ptr, cube->mlx->win_ptr, cube->mlx->map_img, 0, 0);
	return 0;
}

int	mouse_handler(int x, int y, t_cube *cube)
{
	(void)x;

	cube->player_angle += (WINDOW_W / 2. - x) * MOUSE_LUCAS;
	cube->player_dx = cos(cube->player_angle);
	cube->player_dy = sin(cube->player_angle);
	printf("y %d\n", y);
	mlx_mouse_move(cube->mlx->mlx_ptr, cube->mlx->win_ptr, WINDOW_W / 2, WINDOW_H / 2);
	return (0);
}

void	render(t_cube *cube)
{
	copy_playable_map(cube);
	init_player(cube);
	init_keyes(cube);
	cube->mlx->mlx_ptr = mlx_init();
	cube->mlx->win_ptr = mlx_new_window(cube->mlx->mlx_ptr, WINDOW_W, WINDOW_H, "cub3d");
	cube->mlx->map_img = mlx_new_image(cube->mlx->mlx_ptr, cube->map_w*MINIMAP_SCALE, cube->map_h*MINIMAP_SCALE);
	cube->mlx->map_data = mlx_get_data_addr(cube->mlx->map_img, &cube->mlx->map_p_bits, &cube->mlx->map_size_line, &cube->mlx->map_endian);
	cube->mlx->main_img = mlx_new_image(cube->mlx->mlx_ptr, WINDOW_W, WINDOW_H);
	cube->mlx->main_data = mlx_get_data_addr(cube->mlx->main_img, &cube->mlx->main_p_bits, &cube->mlx->main_size_line, &cube->mlx->main_endian);
	mlx_hook(cube->mlx->win_ptr, 17, 0, close_window, cube);
	mlx_hook(cube->mlx->win_ptr, MotionNotify, PointerMotionMask, mouse_handler, cube);
	mlx_hook(cube->mlx->win_ptr, KeyPress,KeyPressMask, key_handler, cube);
	mlx_hook(cube->mlx->win_ptr, KeyRelease,KeyReleaseMask, key_release, cube);
	mlx_loop_hook(cube->mlx->mlx_ptr, put_image, cube);
	mlx_loop(cube->mlx->mlx_ptr);
}
