#include "cub3d.h"

int	key_handler(int keycode, t_cube *cube)
{
	if(keycode == XK_Escape)
		close_window(cube);
	if(keycode == XK_Up || keycode == XK_w)
		cube->keys->key_up = 1;
	if(keycode == XK_Down || keycode == XK_s)
		cube->keys->key_down = 1;
	if(keycode == XK_Left || keycode == XK_a)
		cube->keys->key_left = 1;
	if(keycode == XK_Right || keycode == XK_d)
		cube->keys->key_right = 1;
	if(keycode == XK_o)
		open_door(cube);
	if(keycode == XK_c)
		close_door(cube);
	return (0);
}

int	is_door(t_cube *cube)
{
	if(cube->map[(int)(cube->player_x + cube->radius * cube->player_dx)][(int)(cube->player_y + cube->radius * cube->player_dy)] >= 1001)
		return (1);
	return (0);
}

void	open_door(t_cube *cube)
{
	if(cube->map[(int)(cube->player_x + cube->player_dx)][(int)(cube->player_y + cube->player_dy)] == CLOSE_DOOR)
		cube->map[(int)(cube->player_x + cube->player_dx)][(int)(cube->player_y + cube->player_dy)] += DOOR_SPEED;
}

void	close_door(t_cube *cube)
{
	if(is_door(cube))
		return ;
	if(cube->map[(int)(cube->player_x + cube->player_dx)][(int)(cube->player_y + cube->player_dy)] == OPEN_DOOR)
		cube->map[(int)(cube->player_x + cube->player_dx)][(int)(cube->player_y + cube->player_dy)] -= DOOR_SPEED;
}

int key_release(int keycode, t_cube *cube)
{
	if(keycode == XK_Up || keycode == XK_w)
		cube->keys->key_up = 0;
	if(keycode == XK_Down || keycode == XK_s)
		cube->keys->key_down = 0;
	if(keycode == XK_Left || keycode == XK_a)
		cube->keys->key_left = 0;
	if(keycode == XK_Right || keycode == XK_d)
		cube->keys->key_right = 0;
	return (0);
}

void draw_floor(t_cube *cube)
{
    int x;
    int y;
    int p;

    x = WINDOW_H / 2;
    float plane_x = (cube->player_dy) * tan(FOV / 2);
    float plane_y = (-cube->player_dx) * tan(FOV / 2);
    float ray_dirx = (cube->player_dx) - plane_x;
    float ray_diry = (cube->player_dy) - plane_y;
    float ray_dirx1 = (cube->player_dx) + plane_x;
    float ray_diry1 = (cube->player_dy) + plane_y;
    while (++x < WINDOW_H)
    {
        p = x - WINDOW_H / 2;
        float rowDistance = 0.5 / p;
        float floorStepX = rowDistance * (ray_dirx1 - ray_dirx) / (2 * tan(FOV/2));
        float floorStepY = rowDistance * (ray_diry1 - ray_diry) / (2 * tan(FOV/2));
        float floorX = cube->player_x + rowDistance * ray_dirx * WINDOW_W / (2 * tan(FOV/2));
        float floorY = cube->player_y + rowDistance * ray_diry * WINDOW_W / (2 * tan(FOV/2));
        y = -1;
        while (++y < WINDOW_W)
        {
            int tx = (int)(64 * (floorX - (int)(floorX))) & (64 - 1);
            int ty = (int)(64 * (floorY - (int)(floorY))) & (64 - 1);
            floorX += floorStepX;
            floorY += floorStepY;
            draw_main_pixel(cube, x, y, cube->textures->wall_data[6][64 * ty + tx]);
            draw_main_pixel(cube, WINDOW_H - x - 1, y, cube->textures->wall_data[5][64 * ty + tx]);
        }
    }
}

void door_manager(t_cube *cube)
{
	int x;
	int y;

	x = -1;
	while(++x < cube->map_h)
	{
		y = -1;
		while(++y < cube->map_w)
		{
			if(cube->map[x][y] > CLOSE_DOOR && cube->map[x][y] < OPEN_DOOR && cube->map[x][y] % 2 != 0)
			{
				cube->map[x][y] += 6;
				if(cube->map[x][y] >= OPEN_DOOR)
					cube->map[x][y] = OPEN_DOOR;
			}
			else if(cube->map[x][y] > CLOSE_DOOR && cube->map[x][y] < OPEN_DOOR && cube->map[x][y] % 2 == 0)
			{
				cube->map[x][y] -= 6;
				if(cube->map[x][y] <= CLOSE_DOOR && cube->map[x][y] >= cube->map[x][y] - 6)
					cube->map[x][y] = CLOSE_DOOR;
			}
		}
	}
}

int	mouse_move(int x, int y, t_cube *cube)
{
	(void)x;
	(void)y;
	cube->player_angle += (WINDOW_W / 2. - x) * MOUSE_SENS;
	if(cube->player_angle < 0)
		cube->player_angle += 2*PI;
	if(cube->player_angle > 2*PI)
		cube->player_angle -= 2*PI;
	cube->player_dx = cos(cube->player_angle);
	cube->player_dy = sin(cube->player_angle);
	return (0);
}

int	mouse_click(int button, int x, int y, t_cube *cube)
{
	(void)x;
	(void)y;
	if(button == 1)
		cube->keys->mouse_left = 1;
	return (1);
}

void draw_gun(t_cube *cube, int index)
{
	int x;
	int y;
	unsigned int color;

	x = -1;
	while(++x < cube->anim->h[index])
	{
		y = -1;
		while(++y < cube->anim->w[index])
		{
			color = cube->anim->data[index][x * cube->anim->w[index] + y];
			if(color != 0xff000000)
				draw_main_pixel(cube, x + (WINDOW_H - cube->anim->h[index]), y + (WINDOW_W - cube->anim->w[index]) / 2, color);
		}
	}

}

void animation(t_cube *cube)
{
	if(cube->keys->mouse_left)
		draw_gun(cube, cube->anim->counter++);
	else
		draw_gun(cube, 19);
	if(cube->anim->counter == ANIM_FRAMES - 1)
	{
		cube->keys->mouse_left = 0;
		cube->anim->counter = 0;
	}
}

int put_image(t_cube *cube)
{
	move_player(cube);
	draw_minimap(cube);
	ray_cast(cube);
	door_manager(cube);
	draw_floor(cube);
	draw_walls(cube);
	mlx_mouse_move(cube->mlx->mlx_ptr, cube->mlx->win_ptr, WINDOW_W / 2, WINDOW_H / 2);
	animation(cube);
	mlx_put_image_to_window(cube->mlx->mlx_ptr, cube->mlx->win_ptr, cube->mlx->main_img, 0, 0);
	mlx_put_image_to_window(cube->mlx->mlx_ptr, cube->mlx->win_ptr, cube->mlx->mini_map_img, 0, 0);
	return 0;
}


void load_textures(t_cube *cube)
{
	int i;

	i = -1;
	while(++i < 7)
	{
		cube->textures->wall_ptr[i] = NULL;
		cube->textures->wall_h[i] = 0;
		cube->textures->wall_w[i] = 0;
		cube->textures->wall_p_bits[i] = 0;
		cube->textures->wall_size_line[i] = 0;
		cube->textures->wall_endian[i] = 0;
		cube->textures->wall_data[i] = NULL;
	}
	ft_strlcpy(cube->textures->wall_paths[4], "./textures/door.xpm", ft_strlen("./textures/door.xpm") + 1);
	ft_strlcpy(cube->textures->wall_paths[5], "./textures/floor.xpm", ft_strlen("./textures/floor.xpm") + 1);
	ft_strlcpy(cube->textures->wall_paths[6], "./textures/cealing.xpm", ft_strlen("./textures/cealing.xpm") + 1);
	i = -1;
	while(++i < 7)
	{
		cube->textures->wall_ptr[i] = mlx_xpm_file_to_image(cube->mlx->mlx_ptr, cube->textures->wall_paths[i], &cube->textures->wall_w[i], &cube->textures->wall_h[i]);
		cube->textures->wall_data[i] = (int *)mlx_get_data_addr(cube->textures->wall_ptr[i], &cube->textures->wall_p_bits[i], &cube->textures->wall_size_line[i], &cube->textures->wall_endian[i]);
	}
}

void load_anim(t_cube *cube)
{
    int i;
    char path[64];

    i = -1;
    while(++i < ANIM_FRAMES)
    {
        cube->anim->ptr[i] = NULL;
        cube->anim->h[i] = 0;
        cube->anim->w[i] = 0;
        cube->anim->p_bits[i] = 0;
        cube->anim->size_line[i] = 0;
        cube->anim->endian[i] = 0;
        cube->anim->data[i] = NULL;
        sprintf(path, "anim/gun%d.xpm", i);
        cube->anim->ptr[i] = mlx_xpm_file_to_image(cube->mlx->mlx_ptr, path, &cube->anim->w[i], &cube->anim->h[i]);
        cube->anim->data[i] = (int *)mlx_get_data_addr(cube->anim->ptr[i], &cube->anim->p_bits[i], &cube->anim->size_line[i], &cube->anim->endian[i]);
    }
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
	cube->mlx->mini_map_img = mlx_new_image(cube->mlx->mlx_ptr, 10*MINIMAP_SCALE, 10*MINIMAP_SCALE);
	cube->mlx->mini_map_data = mlx_get_data_addr(cube->mlx->mini_map_img, &cube->mlx->mini_p_bits, &cube->mlx->mini_size_line, &cube->mlx->mini_endian);
	cube->mlx->main_img = mlx_new_image(cube->mlx->mlx_ptr, WINDOW_W, WINDOW_H);
	cube->mlx->main_data = mlx_get_data_addr(cube->mlx->main_img, &cube->mlx->main_p_bits, &cube->mlx->main_size_line, &cube->mlx->main_endian);
	load_anim(cube);
	load_textures(cube);
	mlx_hook(cube->mlx->win_ptr, 17, 0, close_window, cube);
	mlx_hook(cube->mlx->win_ptr, MotionNotify, PointerMotionMask, mouse_move, cube);
	mlx_hook(cube->mlx->win_ptr, KeyPress,KeyPressMask, key_handler, cube);
	mlx_hook(cube->mlx->win_ptr, KeyRelease,KeyReleaseMask, key_release, cube);
	mlx_mouse_hook(cube->mlx->win_ptr, mouse_click, cube);
	mlx_loop_hook(cube->mlx->mlx_ptr, put_image, cube);
	mlx_mouse_hide(cube->mlx->mlx_ptr, cube->mlx->win_ptr);
	mlx_loop(cube->mlx->mlx_ptr);
}
