#include "cub3d.h"

int door_state(int x, int y, t_cube *cube)
{
	int door_value;

	door_value = cube->map[x][y];
	if(door_value >= CLOSE_DOOR && door_value < OPEN_DOOR)
		return (1);
	return (0);
}

int	check_player_position(float player_x, float player_y, t_cube *cube)
{
	if(cube->map[(int)((player_x + cube->radius))][(int)((player_y + cube->radius))] == '#' ||
			(door_state((int)((player_x + cube->radius)), (int)((player_y + cube->radius)), cube)))
		return (0);
	if(cube->map[(int)((player_x + cube->radius))][(int)((player_y - cube->radius))] == '#' ||
			(door_state((int)((player_x + cube->radius)), (int)((player_y - cube->radius)), cube)))
		return (0);
	if(cube->map[(int)((player_x - cube->radius))][(int)((player_y + cube->radius))] == '#' ||
			(door_state((int)((player_x - cube->radius)), (int)((player_y + cube->radius)), cube)))
		return (0);
	if(cube->map[(int)((player_x - cube->radius))][(int)((player_y - cube->radius))] == '#' ||
			(door_state((int)((player_x - cube->radius)), (int)((player_y - cube->radius)), cube)))
		return (0);
	return (1);
}

void	move_player(t_cube *cube)
{
	float move_x;
	float move_y;

	move_x = cube->player_x;
	move_y = cube->player_y;
	if(cube->keys->key_up)
	{
		move_x += cube->player_dx/MINIMAP_SCALE;
		move_y += cube->player_dy/MINIMAP_SCALE;
	}
	else if(cube->keys->key_down)
	{
		move_x -= cube->player_dx/MINIMAP_SCALE;
		move_y -= cube->player_dy/MINIMAP_SCALE;
	}
	if(cube->keys->key_right)
	{
		move_x += cube->player_dy/MINIMAP_SCALE;
		move_y -= cube->player_dx/MINIMAP_SCALE;
	}
	else if(cube->keys->key_left)
	{
		move_x -= cube->player_dy/MINIMAP_SCALE;
		move_y += cube->player_dx/MINIMAP_SCALE;
	}
	if (check_player_position(move_x, cube->player_y, cube))
		cube->player_x = move_x;
	if (check_player_position(cube->player_x, move_y, cube))
		cube->player_y = move_y;
}
