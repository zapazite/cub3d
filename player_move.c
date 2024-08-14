#include "cub3d.h"

int	check_player_position(float player_x, float player_y, t_cube *cube)
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

void	move_player(t_cube *cube)
{
	float move_x;
	float move_y;

	move_x = cube->player_x;
	move_y = cube->player_y;
	if(cube->keys->key_up || cube->keys->key_w)
	{
		move_x += cube->player_dx/MINIMAP_SCALE;
		move_y += cube->player_dy/MINIMAP_SCALE;
	}
	else if(cube->keys->key_down || cube->keys->key_s)
	{
		move_x -= cube->player_dx/MINIMAP_SCALE;
		move_y -= cube->player_dy/MINIMAP_SCALE;
	}
	if(cube->keys->key_right || cube->keys->key_d)
	{
		move_x += cube->player_dy/MINIMAP_SCALE;
		move_y -= cube->player_dx/MINIMAP_SCALE;

	}
	else if(cube->keys->key_left || cube->keys->key_a)
	{
		move_x -= cube->player_dy/MINIMAP_SCALE;
		move_y += cube->player_dx/MINIMAP_SCALE;
	}
	if(check_player_position(move_x, move_y, cube))
	{
		cube->player_x = move_x;
		cube->player_y = move_y;
	}
}
