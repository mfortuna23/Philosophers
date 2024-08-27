/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfortuna <mfortuna@student.42.pt>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 11:39:18 by mfortuna          #+#    #+#             */
/*   Updated: 2024/08/27 12:49:28 by mfortuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int	just_print(t_phil *phil, char *act);
int	change_data(t_phil	*phil, t_op code);

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

int	mutex_create(t_data *data)
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
		return (42);
	if (mutex_create(data) > 0)
		return (42);
	if (argc == 5)
		data->x_eat = -1;
	data->x_eaten = 0;
	data->sim = 1;
	return (0);
}

int	get_info(t_phil *phil, t_op code)
{
	int		info;

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
		info = phil->t_eaten;
		pthread_mutex_unlock(&phil->data->eat);
	}
	return (info);
}

long int	nowtime(t_phil *phil)
{
	long int	seconds;
	long int	micro;
	long int	mil;

	gettimeofday(&phil->data->now, NULL);
	seconds = phil->data->now.tv_sec - phil->data->start.tv_sec;
	micro = phil->data->now.tv_usec - phil->data->start.tv_usec;
	mil = (seconds * 1000) + (micro / 1000);
	return (mil);
}

int	just_print(t_phil *phil, char *act)
{
	if (get_info(phil, SIM) == 1)
	 	return (1);
	pthread_mutex_lock(&phil->data->w);
	if (get_info(phil, SIM) == 1)
		return(pthread_mutex_unlock(&phil->data->w));
	printf("%ld\t%i %s\n",nowtime(phil), phil->id, act);
	pthread_mutex_unlock(&phil->data->w);
	return (get_info(phil, SIM));
}

int	write_now(t_phil *phil, char *act)
{
	if (get_info(phil, SIM) == 1)
		return (1);
	if (ft_strcomp("is eating", act) == 0)
	{
		if (phil->id % 2 != 0)
			pthread_mutex_lock(phil->l_fork);
		else
			pthread_mutex_lock(phil->r_fork);
		if (just_print(phil, "has taken a fork") == 1)
			return (1);
		if (phil->id % 2 != 0)
			pthread_mutex_lock(phil->r_fork);
		else
			pthread_mutex_lock(phil->l_fork);
		if (just_print(phil, "has taken a fork") == 1)
			return (1);
		if (just_print(phil, act) == 1)
			return (1);
		return (0);
	}
	if (just_print(phil, act) == 1)
		return (1);
	return (0);
}
int	new_sleep(t_phil *phil, int time)
{
	while (nowtime(phil) < time)
	{
		if (get_info(phil, SIM) != 0)
			return (1);
	}
	return(get_info(phil, SIM));
}
int	unlock_forks(t_phil *phil)
{
	pthread_mutex_unlock(phil->l_fork);
	pthread_mutex_unlock(phil->r_fork);
	return (1);
}
int	action(t_phil *phil, char *act, int time)
{
	if (get_info(phil, SIM) == 1)
		return (1);
	if (ft_strcomp("is eating", act) == 0)
	{
		if (write_now(phil, act) == 1)
			return (unlock_forks(phil));
		phil->lastmeal = nowtime(phil);
		if (new_sleep(phil, (time /1000) + nowtime(phil)) != 0)
			return (unlock_forks(phil));
		unlock_forks(phil);
		pthread_mutex_lock(&phil->data->eat);
		phil->t_eaten++;
		pthread_mutex_unlock(&phil->data->eat);
		return (get_info(phil, SIM));
	}
	if (write_now(phil, act) == 1)
		return (1);
	if (nowtime(phil) + (time / 1000) >= (phil->lastmeal + (phil->data->t_die / 1000)))
	{
		if (new_sleep(phil, (phil->lastmeal + (phil->data->t_die / 1000))) == 1)
			return (1);
		return (change_data(phil, DIED));
	}
	return(new_sleep(phil, (time / 1000) + nowtime(phil)));
}

int	phil_even(t_phil *phil, int check)
{
	while (get_info(phil, SIM) == 0)
	{
		if (phil->id % 2 != 0)
		{
			if (action(phil, "is eating", phil->data->t_eat) != 0)
				return (1);
		}
		else
		{
			if (phil->t_eaten == 0)
				usleep(1);
			if (action(phil, "is eating", phil->data->t_eat) != 0)
				return (1);
		}
		if (check != 0)
			return (1);
		if (action(phil, "is sleeping", phil->data->t_sleep) != 0)
			return (1);
		if (action(phil, "is thinking", 1) != 0)
			return (1);
	}
	return (0);
}

void	*routine(void *anything)
{
	t_phil	*phil;

	phil = (t_phil *)anything;
	while (get_info(phil, SIM) != 0)
		usleep(1);
	if (phil->id == phil->data->n_phil)
	{
		if (phil->id == 1)
		{
			just_print(phil, "has taken a fork");
			usleep(phil->data->t_die);
			change_data(phil, DIED);
			return (NULL);
		}
		usleep(phil->data->t_eat / 2);
	}
	pthread_mutex_lock(&phil->data->time);
	pthread_mutex_unlock(&phil->data->time);
	phil_even(phil, 0);
	// printf ("\t\t\t\t\t %i sim is %i\n", phil->id, phil->data->sim);
	return (NULL);
}

int	change_data(t_phil	*phil, t_op code)
{
	if (code == SIM)
	{
		pthread_mutex_lock(&phil->data->dead);
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
		phil->t_eaten++;
		pthread_mutex_unlock(&phil->data->eat);
	}
	return (1);
}

void	*phil_dead(t_phil *phil)
{
	change_data(phil, SIM);
	pthread_mutex_lock(&phil->data->w);
	printf("%ld\t%i has died\n",nowtime(phil), phil->id);
	pthread_mutex_unlock(&phil->data->w);
	return (NULL);
}

void *check_pthread(void *anything)
{
	t_data	*data;
	t_phil	*current;

	data = (t_data *)anything;
	current = data->head;
	gettimeofday(&data->start, NULL);
	pthread_mutex_unlock(&data->time);
	data->sim = 0;
	while (data->sim == 0)
	{
		while(current->next != NULL)
		{
			if (get_info(current, DIED) != 0)
				return (phil_dead(current));
			if ((data->x_eat > 0) && get_info(current, EAT) == data->x_eat)
			{
				data->x_eaten++;
				if (data->x_eaten == data->n_phil)
				{
					change_data(current, SIM);
					return (NULL);
				}
			}
			current = current->next;
		}
		current = data->head;
	}
	return (NULL);
}

void	create_phils(t_data *data)
{
	int			i;
	t_phil		*head;
	t_phil		*phil;
	pthread_t	check;

	i = 0;
	head = NULL;
	create_struct(&head, data);
	data->head = head;
	phil = head;
	pthread_mutex_lock(&data->time);
	while (i < data->n_phil)
	{
		pthread_create(&data->phil[i], NULL, &routine, phil);
		phil = phil->next;
		i++;
	}
	pthread_create(&check, NULL, &check_pthread, data);
	i = -1;
	while (++i < data->n_phil)
		pthread_join(data->phil[i], NULL);
	pthread_join(check, NULL);
	clean_struct(&head);
}


int	main(int argc, char **argv)
{
	t_data			data;

	if (argc > 4 && argc < 7)
	{
		if (create_data(&data, argc, argv) > 0)
			return (printf("invalid arguments\n"));
		data.phil = malloc((data.n_phil + 1) * sizeof(pthread_t));
		create_phils(&data);
		// free(data.forks);
		// free(data.phil);
		return (0);
	}
	return (printf("invalid number of arguments \n"));
}
