/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfortuna <mfortuna@student.42.pt>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 14:59:28 by mfortuna          #+#    #+#             */
/*   Updated: 2024/08/23 15:55:02 by mfortuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int	just_print(t_phil *phil, char *act);

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
		if (nbr[i] < '0' && nbr[i] > '9')
			return (-1);
		n = (n * 10) + (nbr[i] - '0');
		i++;
	}
	if (i > 2147483647)
		return (-1);
	return (n);
}

int	mutex_create(t_data *data, t_check *check)
{
	int	i;

	i = 0;
	data->forks = malloc((data->n_phil + 1) * sizeof(pthread_mutex_t));
	while (i < data->n_phil)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			return (printf("no mutex here \n"));
		i++;
	}
	if (pthread_mutex_init(&check->w, NULL) != 0)
		return (printf("no mutex here \n"));
	if (pthread_mutex_init(&check->eat, NULL) != 0)
		return (printf("no mutex here \n"));
	if (pthread_mutex_init(&check->dead, NULL) != 0)
		return (printf("no mutex here \n"));
	if (pthread_mutex_init(&check->time, NULL) != 0)
		return (printf("no mutex here \n"));
	return (0);
}

int	create_data(t_data *data, int argc, char **argv, t_check *check)
{
	data->n_phil = get_number(argv[1]);
	data->t_die = get_number(argv[2]) * 1000;
	data->t_eat = get_number(argv[3]) * 1000;
	data->t_sleep = get_number(argv[4]) * 1000;
	// 60 ms is the minimum
	if (argc == 6)
	{
		data->x_eat = get_number(argv[5]);
		if (data->x_eat < 0)
			return (42);
		if (data->x_eat == 0)
			return (printf("CONGRATS! All the philosophers ate 0 times\n"));
	}
	if (data->n_phil < 0 || data->t_die < 0 || data->t_eat < 0 \
	|| data->t_sleep < 0)
	if (mutex_create(data, check))
		return (42);
	if (argc == 5)
		data->x_eat = -1;
	return (0);
}

int	get_info(pthread_mutex_t mutex, int data)
{
	int		info;

	pthread_mutex_lock(&mutex);
	info = data;
	pthread_mutex_unlock(&mutex);
	return (info);
}

long int	nowtime(t_check *phil)
{
	long int	seconds;
	long int	micro;
	long int	mil;

	gettimeofday(&phil->now, NULL);
	seconds = phil->now.tv_sec - phil->start.tv_sec;
	micro = phil->now.tv_usec - phil->start.tv_usec;
	mil = (seconds * 1000) + (micro / 1000);
	return (mil);
}
void	*routine(void	*anything)
{
	t_phil	*phil;

	phil = (t_phil *)anything;
	if (phil->id == phil->data.n_phil)
	{
		if (phil->id == 1)
		{
			just_print(phil, "has taken a fork");
			usleep(phil->data.t_die);
			phil_dead(phil, 1);
			return (NULL);
		}
		usleep(phil->data.t_eat / 2);
	}
	pthread_mutex_lock(&phil->check->time);
	pthread_mutex_unlock(&phil->check->time);
	phil_even(phil, 0);
	return (NULL);
}

void	*checker(void *anything)
{
	t_check *check;

	check = (t_check *)anything;
	gettimeofday(&check->start, NULL);
	check->sim = true;
	pthread_mutex_unlock(&check->time);
	
}

void	create_phils(t_data data, t_check *check)
{
	int			i;
	t_phil		*head;
	t_phil		*phil;

	i = 0;
	head = NULL;
	create_struct(&head, data, check);
	phil = head;
	pthread_mutex_lock(&check->time);
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
	clean_struct(&head);
}

int	main(int argc, char **argv)
{
	t_data	data;
	t_check	check;

	if (argc > 4 && argc < 7)
	{
		if (create_data(&data, argc, argv, &check) > 0)
			return (printf("invalid arguments\n"));
		data.phil = malloc((data.n_phil + 1) * sizeof(pthread_t));
		create_phils(&data, &check);
		return (0);
	}
	return (printf("invalid number of arguments \n"));
}