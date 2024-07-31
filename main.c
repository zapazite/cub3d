/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-prin <mde-prin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 10:35:49 by mde-prin          #+#    #+#             */
/*   Updated: 2024/07/31 11:29:30 by mde-prin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	parse(char *map_file)
{
	int	fd;
	ssize_t read_size;
	char buf[BUFFERSIZE];


	fd = open(map_file, O_RDONLY);
	if (fd < 0)
		clean_and_exit();
	while (1)
	{
		read_size = read(fd, buf, BUFFERSIZE);
		if (read_size <= 0)
			break;
		buf[read_size] = '\0';
		
	}
}

int	main(int argc, char *argv[])
{
	if (argc > 2)
		return (write(1, "Error\n", 6), 1);
	parse(argv[1]);
	return (0);
}
