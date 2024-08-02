/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfortuna <mfortuna@student.42.pt>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 14:13:55 by mfortuna          #+#    #+#             */
/*   Updated: 2024/08/02 19:43:21 by mfortuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int	get_number(char *nbr)
{
	int		i;
	long	n;

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
		if (nbr[i] < '0' && nbr[i] > '9')
			return (-1);
		n = (n * 10) + (nbr[i] - '0');
		i++;
	}
	if (i > 2147483647)
		return (-1);
	return (n);
}

int	create_data(t_data *data, int argc, char **argv)
{
	pthread_mutex_t	w;

	data->n_phil = get_number(argv[1]);
	data->t_die = get_number(argv[2]);
	data->t_eat = get_number(argv[3]);
	data->t_sleep = get_number(argv[4]);
	if (argc == 6)
	{
		data->x_eat = get_number(argv[5]);
		if (data->x_eat < 0)
			return (42);
	}
	if (data->n_phil < 0 || data->t_die < 0 || data->t_eat < 0 \
	|| data->t_sleep < 0)
		return (42);
	data->w = w;
	if (pthread_mutex_init(&w, NULL) != 0)
		return (printf("no mutex here \n"));
	return (0);
}

void	*routine(void *anything)
{
	int 	i = 0;
	t_phil	*phil;

	phil = (t_phil *)anything;
	while (i < 500)
	{
		pthread_mutex_lock(&phil->data->w);
		printf("philosopher %i is eating\n", phil->id);
		pthread_mutex_unlock(&phil->data->w);
		i++;
	}
	return (NULL);
}

void	create_phils(t_data data)
{
	int		i;
	t_phil	*head;
	t_phil	*phil;

	i = 0;
	head = NULL;
	create_struct(&head, &data);
	phil = head;
	while (i < data.n_phil)
	{
		pthread_create(&data.phil[i], NULL, &routine, phil);
		phil = phil->next;
		i++;
	}
	i = 0;
	while (i < data.n_phil)
	{
		pthread_join(data.phil[i], NULL);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_data			data;


	if (argc > 4 && argc < 7)
	{
		if (create_data(&data, argc, argv) > 0)
			return (printf("invalid arguments\n"));
		data.phil = malloc((data.n_phil + 1) * sizeof(pthread_t));
		create_phils(data);
		free(data.phil);
		return (0);
	}
	return (printf("invalid number of arguments \n"));
}