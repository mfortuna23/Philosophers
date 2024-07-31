/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfortuna <mfortuna@student.42.pt>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 10:08:30 by mfortuna          #+#    #+#             */
/*   Updated: 2024/07/30 11:52:45 by mfortuna         ###   ########.fr       */
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
		printf("philosopher %i is eating\n", phil->id);
		printf("philosopher %i is sleeping\n", phil->id);
		printf("philosopher %i is thinking\n", phil->id);
		i++;
	}
	return (NULL);
}
