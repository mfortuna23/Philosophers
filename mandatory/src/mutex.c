/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfortuna <mfortuna@student.42.pt>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:59:44 by mfortuna          #+#    #+#             */
/*   Updated: 2024/08/30 15:27:55 by mfortuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int	get_info(t_phil *phil, t_op code)
{
	int		info;

	info = -1;
	if (code == SIM)
	{
		pthread_mutex_lock(&phil->data->dead);
		info = phil->data->sim;
		pthread_mutex_unlock(&phil->data->dead);
	}
	else if (code == DIED)
	{
		pthread_mutex_lock(&phil->data->dead);
		info = phil->dead;
		pthread_mutex_unlock(&phil->data->dead);
	}
	else if (code == EAT)
	{
		pthread_mutex_lock(&phil->data->eat);
		info = phil->full;
		pthread_mutex_unlock(&phil->data->eat);
	}
	return (info);
}

int	unlock_forks(t_phil *phil)
{
	pthread_mutex_unlock(phil->l_fork);
	pthread_mutex_unlock(phil->r_fork);
	return (1);
}

int	change_data(t_phil	*phil, t_op code)
{
	if (code == SIM)
	{
		pthread_mutex_lock(&phil->data->dead);
		if (phil->data->sim == 1)
			phil->data->sim = 0;
		else
			phil->data->sim = 1;
		pthread_mutex_unlock(&phil->data->dead);
	}
	if (code == DIED)
	{
		pthread_mutex_lock(&phil->data->dead);
		phil->dead = 1;
		pthread_mutex_unlock(&phil->data->dead);
	}
	if (code == EAT)
	{
		pthread_mutex_lock(&phil->data->eat);
		if (phil->full == 1)
			phil->full = 0;
		else
			phil->full = 1;
		pthread_mutex_unlock(&phil->data->eat);
	}
	return (1);
}

void	*phil_dead(t_phil *phil)
{
	change_data(phil, SIM);
	pthread_mutex_lock(&phil->data->w);
	printf("%ld\t%i has died\n", nowtime(phil->data->start), phil->id);
	pthread_mutex_unlock(&phil->data->w);
	return (NULL);
}

long int	last_meal(t_phil *phil)
{
	long int	lastmeal;

	pthread_mutex_lock(&phil->data->eat);
	lastmeal = phil->lastmeal;
	pthread_mutex_unlock(&phil->data->eat);
	return (lastmeal);
}
