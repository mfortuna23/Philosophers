/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfortuna <mfortuna@student.42.pt>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 10:08:30 by mfortuna          #+#    #+#             */
/*   Updated: 2024/07/26 12:26:46 by mfortuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

void	phil_routine(t_phil *phil)
{
	if (phil->data.n_phil % 2 == 0)
	{
		if (phil->id % 2 != 0)
		{
			phil->prev->rigth_fork = false;
			phil->next->left_fork = false;
		}
	}
}
