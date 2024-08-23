/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world_manager_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efaiz <efaiz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 13:41:16 by efaiz             #+#    #+#             */
/*   Updated: 2024/08/23 13:42:26 by efaiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	world_manager(t_cube *cube)
{
	if (BONUS)
	{
		draw_minimap(cube);
		door_manager(cube);
		draw_c_and_f(cube);
		draw_world(cube);
		animation(cube);
	}
	else
		draw_basic_world(cube);
}
