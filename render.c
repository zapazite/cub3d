/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efaiz <efaiz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 13:31:48 by efaiz             #+#    #+#             */
/*   Updated: 2024/08/23 15:24:41 by efaiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// void draw_c_and_f(t_cube *cube)
// {
//     int x;
//     int y;
//     float floorX;
//     float floorY;

//     x = WINDOW_H / 2;
//     while (++x < WINDOW_H)
//     {
//     	floorX = cube->player_x + ((x - WINDOW_H / 2.)) * ((cube->player_dx) - (cube->player_dy) * tan(FOV / 2)) * WINDOW_W / (4 * tan(FOV/2));
//      	floorY = cube->player_y + ((x - WINDOW_H / 2.)) * ((cube->player_dy) + (cube->player_dx) * tan(FOV / 2)) * WINDOW_W / (4 * tan(FOV/2));
//         y = -1;
//         while (++y < WINDOW_W)
//         {
//             floorX += (0.5 / (x - WINDOW_H / 2.)) * cube->player_dy;
//             floorY += (0.5 / (x - WINDOW_H / 2.)) * -cube->player_dx;
//             draw_main_pixel(cube, x, y, cube->txt->data[5][64 * ((int)(64 * (floorY - (int)(floorY))) & (64 - 1)) + ((int)(64 * (floorX - (int)(floorX))) & (64 - 1))]);
//             draw_main_pixel(cube, WINDOW_H - x - 1, y, cube->txt->data[6][64 * ((int)(64 * (floorY - (int)(floorY))) & (64 - 1)) + ((int)(64 * (floorX - (int)(floorX))) & (64 - 1))]);
//         }
//     }
// }

void draw_c_and_f(t_cube *cube)
{
    int x;
    int y;
    int p;

    x = WINDOW_H / 2;
    float ray_dirx = (cube->player_dx) - (cube->player_dy) * tan(FOV / 2);
    float ray_diry = (cube->player_dy) - (-cube->player_dx) * tan(FOV / 2);
    float ray_dirx1 = (cube->player_dx) + (cube->player_dy) * tan(FOV / 2);
    float ray_diry1 = (cube->player_dy) + (-cube->player_dx) * tan(FOV / 2);
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
            draw_main_pixel(cube, x, y, cube->txt->data[5][64 * ty + tx]);
            draw_main_pixel(cube, WINDOW_H - x - 1, y, cube->txt->data[6][64 * ty + tx]);
        }
    }
}

int	put_image(t_cube *cube)
{
	move_player(cube);
	ray_cast(cube);
	world_manager(cube);
	mlx_mouse_move(cube->mlx->mlx_ptr, cube->mlx->win_ptr, WINDOW_W / 2,
		WINDOW_H / 2);
	mlx_put_image_to_window(cube->mlx->mlx_ptr, cube->mlx->win_ptr,
		cube->mlx->main_img, 0, 0);
	if (BONUS)
		mlx_put_image_to_window(cube->mlx->mlx_ptr, cube->mlx->win_ptr,
			cube->mlx->mini_map_img, 0, 0);
	return (0);
}

void	load_anim(t_cube *cube)
{
	int		i;
	char	path[64];

	i = -1;
	while (++i < ANIM_FRAMES)
	{
		cube->anim->ptr[i] = NULL;
		cube->anim->h[i] = 0;
		cube->anim->w[i] = 0;
		cube->anim->p_bits[i] = 0;
		cube->anim->size_line[i] = 0;
		cube->anim->endian[i] = 0;
		cube->anim->data[i] = NULL;
		sprintf(path, "anim/gun%d.xpm", i);
		cube->anim->ptr[i] = mlx_xpm_file_to_image(cube->mlx->mlx_ptr, path,
				&cube->anim->w[i], &cube->anim->h[i]);
		cube->anim->data[i] = (int *)mlx_get_data_addr(cube->anim->ptr[i],
				&cube->anim->p_bits[i], &cube->anim->size_line[i],
				&cube->anim->endian[i]);
	}
}

void	init_mlx(t_cube *cube)
{
	cube->mlx->mlx_ptr = mlx_init();
	cube->mlx->win_ptr = mlx_new_window(cube->mlx->mlx_ptr, WINDOW_W, WINDOW_H,
			"cub3d");
	cube->mlx->map_img = mlx_new_image(cube->mlx->mlx_ptr, cube->map_w
			* WINDOW_W / 80, cube->map_h * WINDOW_W / 80);
	cube->mlx->map_data = mlx_get_data_addr(cube->mlx->map_img,
			&cube->mlx->map_p_bits, &cube->mlx->map_size_line,
			&cube->mlx->map_endian);
	cube->mlx->mini_map_img = mlx_new_image(cube->mlx->mlx_ptr, 10
			* WINDOW_W / 80, 10 * WINDOW_W / 80);
	cube->mlx->mini_map_data = mlx_get_data_addr(cube->mlx->mini_map_img,
			&cube->mlx->mini_p_bits, &cube->mlx->mini_size_line,
			&cube->mlx->mini_endian);
	cube->mlx->main_img = mlx_new_image(cube->mlx->mlx_ptr, WINDOW_W, WINDOW_H);
	cube->mlx->main_data = mlx_get_data_addr(cube->mlx->main_img,
			&cube->mlx->main_p_bits, &cube->mlx->main_size_line,
			&cube->mlx->main_endian);
}

void	render(t_cube *cube)
{
	copy_playable_map(cube);
	init_player(cube);
	init_keyes(cube);
	init_mlx(cube);
	if (BONUS)
		load_anim(cube);
	load_textures(cube);
	mlx_hook(cube->mlx->win_ptr, 17, 0, close_window, cube);
	mlx_hook(cube->mlx->win_ptr, MotionNotify, PointerMotionMask, mouse_move,
		cube);
	mlx_hook(cube->mlx->win_ptr, KeyPress, KeyPressMask, key_handler, cube);
	mlx_hook(cube->mlx->win_ptr, KeyRelease, KeyReleaseMask, key_release, cube);
	mlx_mouse_hook(cube->mlx->win_ptr, mouse_click, cube);
	mlx_loop_hook(cube->mlx->mlx_ptr, put_image, cube);
	mlx_mouse_hide(cube->mlx->mlx_ptr, cube->mlx->win_ptr);
	mlx_loop(cube->mlx->mlx_ptr);
}
