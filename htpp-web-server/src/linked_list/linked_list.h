/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaitouna <aaitouna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 00:31:59 by aaitouna          #+#    #+#             */
/*   Updated: 2023/01/25 00:51:06 by aaitouna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct s_list
{
    char *key;
    char *value;
    struct s_list *next;
} t_list;

#include <stdlib.h>
void ft_lstadd_back(t_list **lst, t_list *new);
t_list	*ft_lstnew(char *key, void *value);
void	ft_lstclear(t_list **lst, void (*del)(void *));
#endif