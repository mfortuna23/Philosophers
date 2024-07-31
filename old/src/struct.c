/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfortuna <mfortuna@student.42.pt>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 11:01:12 by mfortuna          #+#    #+#             */
/*   Updated: 2024/07/31 13:42:44 by mfortuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

void	delete_stack(t_phil **head)
{
	while (*head != NULL)
		delete_node(head);
}

void	delete_node(t_phil **head)
{
	t_phil	*tmp;
	t_phil	*last;

	if ((*head) == NULL)
		return ;
	if ((*head)->next == NULL)
	{
		free(*head);
		*head = NULL;
		return ;
	}
	tmp = *head;
	while (tmp->next->id < (*head)->data.n_phil)
		tmp = tmp->next;
	last = tmp->next;
	tmp->next = NULL;
	last->prev = NULL;
	free(last);
	(*head)->data.n_phil--;
	return ;
}

void	create_struct(t_phil **head, t_data data, pthread_mutex_t *write_in)
{
	int	i;

	i = 1;
	*head = NULL;
	while (i <= data.n_phil)
	{
		add_node(head, i, data, write_in);
		i++;
	}
}

void	add_node(t_phil **head, int nbr, t_data data, pthread_mutex_t *write_in)
{
	t_phil	*last;
	t_phil	*new_node;

	new_node = create_node(nbr, data, write_in);
	if (*head == NULL)
	{
		*head = new_node;
		return ;
	}
	last = *(head);
	while (last->next != NULL)
		last = last->next;
	last->next = new_node;
	new_node->prev = last;
	if (nbr == data.n_phil)
	{
		new_node->next = *(head);
		new_node->next->prev = new_node;
	}
}

t_phil	*create_node(int nbr, t_data data, pthread_mutex_t *write_in)
{
	t_phil	*node;

	node = malloc(sizeof(t_phil));
	node->data = data;
	node->next = NULL;
	node->prev = NULL;
	node->write_in = write_in;
	node->id = nbr;
	node->left_fork = true;
	node->rigth_fork = true;
	node->eat = false;
	node->sleep = false;
	node->think = false;
	return (node);
}
