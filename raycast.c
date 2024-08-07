/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-prin <mde-prin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:29:43 by mde-prin          #+#    #+#             */
/*   Updated: 2024/08/07 08:35:12 by mde-prin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include "cub3d.h"

typedef union
{
    int32_t i;     // 32-bit integer representing the fixed-point number
    struct {
        int16_t lo; // 16-bit fractional part (lower bits)
        int16_t hi; // 16-bit integer part (higher bits)
    };
} fixed_point;

void bresenham_fixed_point(t_cube *cube, int dir_x, int dir_y)
{
	float	slope;
	float	start_x;
	float	start_y;

	slope = dir_y / dir_x;

	if ((((int)(cube->player_y + (dir_y > 0)) - cube->player_y) / ((int)(cube->player_x + (dir_x > 0)) - cube->player_x)) > slope)
	{
		start_x = (int)(cube->player_x + (dir_x > 0));
		start_y = ((int)(cube->player_x + (dir_x > 0)) - cube->player_x) * slope;
	}

    fixed_point f;
    int x;

    // Calculate dy_fix as fixed-point
    int32_t dy_fix = ((int32_t)(y2 - y1)) << 16;

    // Calculate slope m as fixed-point
    int32_t dx = x2 - x1;
    int32_t m = (dx != 0) ? dy_fix / dx : 0; // Ensure dx is not zero before division

    // Initialize f.i with y1 in fixed-point format
    f.i = y1 << 16;

    // Iterate through x coordinates from x1 to x2
    for (x = x1; x <= x2; x++, f.i += m) {
        fixed_point g = f;
        g.i += 32767; // Add rounding adjustment

        // Check if the grid at (x, g.hi >> 16) is blocking
        if (grid[g.hi][x] == BLOCKING) {
            return;
        }
    }
}