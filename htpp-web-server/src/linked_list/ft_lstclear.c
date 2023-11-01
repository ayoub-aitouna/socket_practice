/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaitouna <aaitouna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 14:41:01 by aaitouna          #+#    #+#             */
/*   Updated: 2023/01/25 00:53:45 by aaitouna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "linked_list.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*temp;

	if (!lst | !del)
		return ;
	while ((*lst))
	{
		temp = (*lst)->next;
		// ft_lstdelone(*lst, del);
		(*lst) = temp;
	}
	lst = 0;
}
