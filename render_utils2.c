/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efaiz <efaiz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 15:17:56 by efaiz             #+#    #+#             */
/*   Updated: 2024/08/23 15:20:57 by efaiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_txt(t_cube *cube)
{
	int	i;

	i = -1;
	while (++i < 7)
	{
		cube->txt->ptr[i] = NULL;
		cube->txt->h[i] = 0;
		cube->txt->w[i] = 0;
		cube->txt->p_bits[i] = 0;
		cube->txt->size_line[i] = 0;
		cube->txt->endian[i] = 0;
		cube->txt->data[i] = NULL;
	}
}

void	load_textures(t_cube *cube)
{
	int	i;

	i = -1;
	ft_strlcpy(cube->txt->paths[4], "./txt/door.xpm",
		ft_strlen("./txt/door.xpm") + 1);
	ft_strlcpy(cube->txt->paths[5], "./txt/lava.xpm",
		ft_strlen("./txt/lava.xpm") + 1);
	ft_strlcpy(cube->txt->paths[6], "./txt/cealing.xpm",
		ft_strlen("./txt/cealing.xpm") + 1);
	while (++i < 7)
	{
		cube->txt->ptr[i] = mlx_xpm_file_to_image(cube->mlx->mlx_ptr,
				cube->txt->paths[i], &cube->txt->w[i], &cube->txt->h[i]);
		cube->txt->data[i] = (int *)mlx_get_data_addr(cube->txt->ptr[i],
				&cube->txt->p_bits[i], &cube->txt->size_line[i],
				&cube->txt->endian[i]);
	}
}

void	door_manager_1(t_cube *cube, int x, int y)
{
	if (cube->map[x][y] > CLOSE_DOOR && cube->map[x][y] < OPEN_DOOR
		&& cube->map[x][y] % 2 != 0)
	{
		cube->map[x][y] += 6;
		if (cube->map[x][y] >= OPEN_DOOR)
			cube->map[x][y] = OPEN_DOOR;
	}
}

void	door_manager(t_cube *cube)
{
	int	x;
	int	y;

	x = -1;
	while (++x < cube->map_h)
	{
		y = -1;
		while (++y < cube->map_w)
		{
			door_manager_1(cube, x, y);
			if (cube->map[x][y] > CLOSE_DOOR && cube->map[x][y] < OPEN_DOOR
				&& cube->map[x][y] % 2 == 0)
			{
				cube->map[x][y] -= 6;
				if (cube->map[x][y] <= CLOSE_DOOR
					&& cube->map[x][y] >= cube->map[x][y] - 6)
					cube->map[x][y] = CLOSE_DOOR;
			}
		}
	}
}

int	mouse_move(int x, int y, t_cube *cube)
{
	if (!BONUS)
		return (0);
	(void)x;
	(void)y;
	cube->player_angle += (WINDOW_W / 2. - x) * MOUSE_SENS;
	if (cube->player_angle < 0)
		cube->player_angle += 2 * PI;
	if (cube->player_angle > 2 * PI)
		cube->player_angle -= 2 * PI;
	cube->player_dx = cos(cube->player_angle);
	cube->player_dy = sin(cube->player_angle);
	return (0);
}
