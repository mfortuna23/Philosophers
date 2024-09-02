/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfortuna <mfortuna@student.42.pt>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:56:08 by mfortuna          #+#    #+#             */
/*   Updated: 2024/09/02 11:41:00 by mfortuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

long int	get_number(char *nbr)
{
	long int	i;
	long int	n;

	i = 0;
	n = 0;
	while (nbr[i] == '-' || nbr[i] == '+')
	{
		if (nbr[i] == '-')
			return (-1);
		i++;
	}
	while (nbr[i])
	{
		if (nbr[i] < '0' || nbr[i] > '9')
			return (-1);
		n = (n * 10) + (nbr[i] - '0');
		i++;
	}
	if (i > 2147483647)
		return (-1);
	return (n);
}

int	mutex_create(t_data *data)
{
	int	i;

	i = 0;
	data->forks = malloc((data->n_phil) * sizeof(pthread_mutex_t));
	while (i < data->n_phil)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			return (printf("no mutex here \n"));
		i++;
	}
	if (pthread_mutex_init(&data->w, NULL) != 0)
		return (printf("no mutex here \n"));
	if (pthread_mutex_init(&data->eat, NULL) != 0)
		return (printf("no mutex here \n"));
	if (pthread_mutex_init(&data->dead, NULL) != 0)
		return (printf("no mutex here \n"));
	if (pthread_mutex_init(&data->time, NULL) != 0)
		return (printf("no mutex here \n"));
	return (0);
}

int	create_data(t_data *data, int argc, char **argv)
{
	data->n_phil = get_number(argv[1]);
	data->t_die = get_number(argv[2]) * 1000;
	data->t_eat = get_number(argv[3]) * 1000;
	data->t_sleep = get_number(argv[4]) * 1000;
	if (argc == 6)
	{
		data->x_eat = get_number(argv[5]);
		if (data->x_eat < 0)
			return (42);
		if (data->x_eat == 0)
			return (printf("CONGRATS! All the philoss ate 0 times\n"));
	}
	if (data->n_phil < 1 || data->t_die < 61 || data->t_eat < 61 \
	|| data->t_sleep < 61)
		return (42);
	if (mutex_create(data) > 0)
		return (42);
	if (argc == 5)
		data->x_eat = -1;
	data->x_eaten = 0;
	data->sim = 1;
	return (0);
}
