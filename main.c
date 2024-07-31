/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efaiz <efaiz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 10:35:49 by mde-prin          #+#    #+#             */
/*   Updated: 2024/07/31 13:57:22 by efaiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	ft_strncmp(const char *str1, const char *str2, size_t n)
{
	int	i;

	i = -1;
	if (n == 0)
		return (0);
	while (n-- > 0)
	{
		if (str1[++i] != *str2)
			return ((unsigned char)str1[i] - *(unsigned char *)str2);
		str2++;
		if (str1[i] == '\0')
			return (0);
	}
	return (0);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

int		check_param(char *line)
{
	int j;
	const char *param_array[] = {"NO ", "SO ", "WE ", "EA "};
	const char	*color_array[] = {"F ", "C "};

	if(line[0] == '\n')
		return (0);
	j = -1;
	while(param_array[++j])
		if(ft_strncmp(line, param_array[j], ft_strlen(param_array[j])))
			return (1);
	j = -1;
	while(color_array[++j])
		if(ft_strncmp(line, color_array[j], ft_strlen(color_array[j])))
			return (1);
	if (j < 4)
		check_path();
	else
		check_rgb();
}

void	parse_parameter(t_cube *cube,int fd)
{
	char	*line;
	int		i;
	int		param_counter;


	line = NULL;
	param_counter = 0;
	i = 0;
	line = get_next_line(cube, fd);
	while(line)
	{
		if(line[0] == '\n')
			;
		else if (param_counter < 6)
			param_counter += check_param(line + i);
		line = get_next_line(cube, fd);
	}
}

void	parse_map(t_cube *cube,int fd)
{
	char	*line;

	line = NULL;
	line = get_next_line(cube, fd);
	while(line)
	{
		line = get_next_line(cube, fd);
	}
}

void	parse(t_cube *cube, char *map_file)
{
	int		fd;
	fd = open(map_file, O_RDONLY);
	if (fd < 0)
		clean_and_exit();
	parse_parameter(cube, fd);
	parse_map(cube, fd);

}

void cube_init(t_cube *cube)
{
	cube->gc = NULL;
}

int	main(int argc, char *argv[])
{
	t_cube cube;

	if (argc > 2)
		return (write(1, "Error\n", 6), 1);
	cube_init(&cube);
	parse(&cube, argv[1]);
	return (0);
}
