/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfortuna <mfortuna@student.42.pt>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 11:25:33 by mfortuna          #+#    #+#             */
/*   Updated: 2024/08/30 11:58:31 by mfortuna         ###   ########.fr       */
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

long int	nowtime(struct timeval start)
{
	long int		seconds;
	long int		micro;
	long int		mil;
	struct timeval now;

	gettimeofday(&now, NULL);
	seconds = now.tv_sec - start.tv_sec;
	micro = now.tv_usec - start.tv_usec;
	mil = (seconds * 1000) + (micro / 1000);
	return (mil);
}

int	just_print(t_phil *phil, char *act)
{
	if (get_info(phil, SIM) == 1)
	 	return (1);
	pthread_mutex_lock(&phil->data->w);
	if (get_info(phil, SIM) == 1)
	{
		pthread_mutex_unlock(&phil->data->w);
		return (1);
	}
	printf("%ld\t%i %s\n",nowtime(phil->data->start), phil->id, act);
	pthread_mutex_unlock(&phil->data->w);
	return (get_info(phil, SIM));
}

int	write_now(t_phil *phil, char *act)
{
	if (get_info(phil, SIM) == 1)
		return (1);
	if (ft_strcomp("is eating", act) == 0)
	{
		pthread_mutex_lock(phil->r_fork);
		if (just_print(phil, "has taken a fork") == 1)
		{
			pthread_mutex_unlock(phil->r_fork);
			return (1);
		}
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
	while (nowtime(phil->data->start) < time)
	{
		if ((phil->lastmeal + (phil->data->t_die / 1000)) <= nowtime(phil->data->start))
			return(change_data(phil, DIED));
		if (get_info(phil, SIM) != 0)
			return (1);
	}
	// printf("%i when to die %ld, now : %ld\n", phil->id, (phil->lastmeal + (phil->data->t_die / 1000)), nowtime(phil));
	return(get_info(phil, SIM));
}
int	unlock_forks(t_phil *phil)
{
	pthread_mutex_unlock(phil->l_fork);
	pthread_mutex_unlock(phil->r_fork);
	return (1);
}
int	action(t_phil *phil, char *act, long time)
{
	if (get_info(phil, SIM) == 1)
		return (1);
	if (ft_strcomp("is eating", act) == 0)
	{
		if (write_now(phil, act) == 1)
			return (1);
		pthread_mutex_lock(&phil->data->eat);
		phil->lastmeal = nowtime(phil->data->start);
		pthread_mutex_unlock(&phil->data->eat);
		if (new_sleep(phil, (time /1000) + nowtime(phil->data->start)) != 0)
			return (unlock_forks(phil));
		unlock_forks(phil);
		phil->t_eaten++;
		if (phil->t_eaten == phil->data->x_eat)
			change_data(phil, EAT);
		return (get_info(phil, SIM));
	}
	if (write_now(phil, act) == 1)
		return (1);
	// printf("id: %i | now : %ld | time : %ld | last_meal %ld | t_die %ld\n",phil->id, nowtime(phil), (time / 1000), phil->lastmeal, (phil->data->t_die / 1000));
	if (nowtime(phil->data->start) + (time / 1000) >= (phil->lastmeal + (phil->data->t_die / 1000)))
	{
		if (new_sleep(phil, (phil->lastmeal + (phil->data->t_die / 1000))) == 1)
			return (1);
		return (change_data(phil, DIED));
	}
	return(new_sleep(phil, (time / 1000) + nowtime(phil->data->start)));
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
			usleep(1000);
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
	// pthread_mutex_lock(&phil->data->time);
	// pthread_mutex_unlock(&phil->data->time);
	phil_even(phil, 0);
	// printf ("\t\t\t\t\t %i sim is %i\n", phil->id, phil->data->sim);
	return (NULL);
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
	printf("%ld\t%i has died\n",nowtime(phil->data->start), phil->id);
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

void *check_pthread(void *anything)
{
	t_data	*data;
	t_phil	*current;

	data = (t_data *)anything;
	current = data->head;
	gettimeofday(&data->start, NULL);
	// pthread_mutex_unlock(&data->time);
	change_data(current, SIM);
	while (data->sim == 0)
	{
		while(current != NULL)
		{
			if (get_info(current, DIED) != 0 || (last_meal(current) + (data->t_die / 1000)) <= nowtime(data->start))
				return (phil_dead(current));
			if ((data->x_eat > 0) && get_info(current, EAT) == 1)
			{
				data->x_eaten++;
				change_data(current, EAT);
				// printf("quantos comeram tudo %ld \n", data->x_eaten);
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
