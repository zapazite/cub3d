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

int player_dir(int dx, int dy)
{
	if (dx == 0 && dy == -1)
		return(0);
    if (dx == 0 && dy == 1)
    	return(0);
    if (dx == -1 && dy == 0)
    	return (0);
    if (dx == 1 && dy == 0)
    	return (0);
    else
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
	//needs to be fixed, player getting stuck in walls
	if(check_player_position(move_x, move_y, cube))
	{
		if(cube->map[(int)cube->player_x][(int)(cube->player_dy + move_y)] == '#')
			cube->player_x = move_x;
		else if(cube->map[(int)(cube->player_dx + move_x)][(int)cube->player_y] == '#')
			cube->player_y = move_y;
	}
	else
	{
		printf("move_x = %f\n", move_x);
		cube->player_x = move_x;
		cube->player_y = move_y;
	}
}
