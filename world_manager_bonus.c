#include "cub3d.h"

void world_manager(t_cube *cube)
{
	if(BONUS)
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
