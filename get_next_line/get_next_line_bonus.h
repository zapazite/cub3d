/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-prin <mde-prin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 16:43:35 by mde-prin          #+#    #+#             */
/*   Updated: 2024/07/31 12:24:26 by mde-prin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 8192
# endif
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stddef.h>
# include "../gc/gc.h"

typedef struct s_cube	t_cube;

typedef struct s_list
{
	char			letter;
	struct s_list	*next;
}	t_list;

t_list	*ft_lstnew(t_cube *cube, char letter);
char	*get_next_line(t_cube *cube, int fd);
char	*lst_to_str(t_cube *cube, t_list **lst);
void	fill_lst(t_cube *cube, t_list **lst, t_list *last, int fd);
int		get_nl(t_list *lst);
int		get_len(t_list *lst, int i);
t_list	*last(t_list *lst);
#endif
