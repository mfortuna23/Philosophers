/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfortuna <mfortuna@student.42.pt>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 15:02:33 by mfortuna          #+#    #+#             */
/*   Updated: 2024/08/30 15:55:14 by mfortuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int	sim_continue(t_phil *current, t_data *data)
{
	if (get_info(current, DIED) != 0 || (last_meal(current) + \
	(data->t_die / 1000)) <= nowtime(data->start))
	{
		phil_dead(current);
		return (1);
	}
	if ((data->x_eat > 0) && get_info(current, EAT) == 1)
	{
		data->x_eaten++;
		change_data(current, EAT);
		if (data->x_eaten == data->n_phil)
		{
			change_data(current, SIM);
			return (1);
		}
	}
	return (0);
}

void	*check_pthread(void *anything)
{
	t_data	*data;
	t_phil	*current;

	data = (t_data *)anything;
	current = data->head;
	gettimeofday(&data->start, NULL);
	change_data(current, SIM);
	while (data->sim == 0)
	{
		while (current != NULL)
		{
			if (sim_continue(current, data) == 1)
				return (NULL);
			current = current->next;
		}
		current = data->head;
		usleep(50);
	}
	return (NULL);
}

int	update_meal(t_phil *phil, long int time)
{
	pthread_mutex_lock(&phil->data->eat);
	phil->lastmeal = nowtime(phil->data->start);
	pthread_mutex_unlock(&phil->data->eat);
	if (new_sleep(phil, (time / 1000) + nowtime(phil->data->start)) != 0)
		return (unlock_forks(phil));
	unlock_forks(phil);
	return (0);
}
