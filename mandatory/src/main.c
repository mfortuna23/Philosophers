/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfortuna <mfortuna@student.42.pt>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 11:25:33 by mfortuna          #+#    #+#             */
/*   Updated: 2024/08/30 15:09:21 by mfortuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int	main(int argc, char **argv)
{
	t_data	data;
	int		i;

	i = 0;
	if (argc > 4 && argc < 7)
	{
		if (create_data(&data, argc, argv) > 0)
			return (printf("invalid arguments\n"));
		data.phil = malloc((data.n_phil + 1) * sizeof(pthread_t));
		create_phils(&data);
		while (i < data.n_phil)
		{
			pthread_mutex_destroy(&data.forks[i]);
			i++;
		}
		free(data.forks);
		free(data.phil);
		pthread_mutex_destroy(&data.w);
		pthread_mutex_destroy(&data.eat);
		pthread_mutex_destroy(&data.dead);
		pthread_mutex_destroy(&data.time);
		return (0);
	}
	return (printf("invalid number of arguments \n"));
}
