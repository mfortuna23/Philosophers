/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfortuna <mfortuna@student.42.pt>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 10:08:30 by mfortuna          #+#    #+#             */
/*   Updated: 2024/07/31 14:03:50 by mfortuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

void	*phil_routine(void *node)
{
	int i = 0;
	t_phil *phil;
	phil = (t_phil *)node;
	while (i < 6)
	{
		pthread_mutex_lock(phil->write_in);
		printf("%i philosopher %i is eating\n", i, phil->id);
		pthread_mutex_unlock(phil->write_in);
		pthread_mutex_lock(phil->write_in);
		printf("%i philosopher %i is sleeping\n", i, phil->id);
		pthread_mutex_unlock(phil->write_in);
		pthread_mutex_lock(phil->write_in);
		printf("%i philosopher %i is thinking\n", i, phil->id);
		pthread_mutex_unlock(phil->write_in);
		i++;
	}
	return (NULL);
}
