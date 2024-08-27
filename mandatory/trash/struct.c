/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfortuna <mfortuna@student.42.pt>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 14:58:59 by mfortuna          #+#    #+#             */
/*   Updated: 2024/08/27 10:12:19 by mfortuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

t_phil	*create_node(t_data data, int id, t_check *check)
{
	t_phil	*node;

	node = malloc(sizeof(t_phil));
	node->id = id;
	if (id == 1)
		node->r_fork = &data.forks[data.n_phil - 1];
	else
		node->r_fork = &data.forks[id - 2];
	node->l_fork = &data.forks[id - 1];
	node->t_eaten = 0;
	node->lastmeal = 0;
	node->data = data;
	node->check = check;
	node->next = NULL;
	return (node);
}

void	add_last(t_phil **head, t_data data, int id, t_check *check)
{
	t_phil	*current;

	if ((*head) == NULL)
	{
		(*head) = create_node(data, id, check);
		return ;
	}
	current = *head;
	if (current->next == NULL)
	{
		current->next = create_node(data, id, check);
		return ;
	}
	while (current->next != NULL)
		current = current->next;
	current->next = create_node(data, id, check);
}

void	create_struct(t_phil **head, t_data data, t_check *check)
{
	int		i;

	i = 0;
	while (i < data.n_phil)
	{
		add_last(head, data, i + 1, check);
		i++;
	}
}
void	delete_last(t_phil **head)
{
	t_phil	*last;
	t_phil	*del;

	last = (*head);
	while (last->next->next != NULL)
		last = last->next;
	del = last->next;
	last->next = NULL;
	free (del);
	return ;
}


void	clean_struct(t_phil **head)
{
	while ((*head)->next != NULL)
		delete_last(head);
	free(*head);
	return ;
}
