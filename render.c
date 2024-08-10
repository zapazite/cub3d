#include "cub3d.h"

int	key_handler(int keycode, t_cube *cube)
{
	if(keycode == XK_Escape)
		close_window(cube);
	if(keycode == XK_Up || keycode == XK_Down)
		move_player(keycode, cube);
	if(keycode == XK_Left || keycode == XK_Right)
		rotate_player(keycode, cube);
	return (0);
}

int put_image(t_cube *cube)
{
	draw_minimap(cube);
	draw_player(cube);
	ray_cast(cube);
	mlx_put_image_to_window(cube->mlx->mlx_ptr, cube->mlx->win_ptr, cube->mlx->image, 0, 0);
	return 0;
}

void	render(t_cube *cube)
{
	copy_playable_map(cube);
	cube->mlx->mlx_ptr = mlx_init();
	cube->mlx->win_ptr = mlx_new_window(cube->mlx->mlx_ptr, WINDOW_W, WINDOW_H, "diplsay mini_map");
	cube->mlx->image = mlx_new_image(cube->mlx->mlx_ptr, cube->map_w*MINIMAP_SCALE, cube->map_h*MINIMAP_SCALE);
	cube->mlx->img_data = mlx_get_data_addr(cube->mlx->image, &cube->mlx->pixel_bits, &cube->mlx->size_line, &cube->mlx->endian);
	init_player(cube);
	init_ray(cube);
	mlx_hook(cube->mlx->win_ptr, 17, 0, close_window, cube);
	mlx_hook(cube->mlx->win_ptr, KeyPress,KeyPressMask, key_handler, cube);
	mlx_loop_hook(cube->mlx->mlx_ptr, put_image, cube);
	mlx_loop(cube->mlx->mlx_ptr);
}
