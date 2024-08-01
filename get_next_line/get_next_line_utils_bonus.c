/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-prin <mde-prin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 16:43:18 by mde-prin          #+#    #+#             */
/*   Updated: 2024/08/01 11:38:35 by mde-prin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

t_list	*ft_lstnew(t_cube *cube, char letter)
{
	t_list	*lst;

	lst = (t_list *)ft_malloc(cube, sizeof(t_list));
	lst->letter = letter;
	lst->next = NULL;
	return (lst);
}

t_list	*last(t_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

int	get_nl(t_list *lst)
{
	while (lst && lst->letter != '\n')
		lst = lst->next;
	return (lst != NULL);
}

int	get_len(t_list *lst, int i)
{
	if (!lst)
		return (0);
	while (lst)
	{
		i++;
		if (lst->letter == '\n')
			return (i);
		lst = lst->next;
	}
	return (i);
}
