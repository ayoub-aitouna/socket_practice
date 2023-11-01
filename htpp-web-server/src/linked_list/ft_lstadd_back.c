/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaitouna <aaitouna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 14:40:33 by aaitouna          #+#    #+#             */
/*   Updated: 2023/01/25 00:33:33 by aaitouna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "linked_list.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*temp;

	temp = *lst;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	while (temp)
	{
		if ((temp)->next == NULL)
			break ;
		temp = (temp)->next;
	}
	(temp)->next = new;
}
