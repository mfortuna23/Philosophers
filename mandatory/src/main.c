/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfortuna <mfortuna@student.42.pt>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 14:13:55 by mfortuna          #+#    #+#             */
/*   Updated: 2024/08/20 15:03:35 by mfortuna         ###   ########.fr       */
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
	data->died = 0;
	data->eaten = 0;
	return (0);
}
int		get_info(pthread_mutex_t mutex, int data)
{
	int		info;

	pthread_mutex_lock(&mutex);
	info = data;
	pthread_mutex_unlock(&mutex);
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

int	phil_dead(t_phil *phil, int code)
{
	if (get_info(phil->data->dead, phil->data->died) == 1){
		// just_print(phil, "\t\t\t phil_dead 1");
		return (1);}
	if (code == 1)
		just_print(phil, "has died");
	// just_print(phil, "\t\t\t phil_dead 2");
	pthread_mutex_lock(&phil->data->dead);
	phil->data->died = 1;
	pthread_mutex_unlock(&phil->data->dead);
	// just_print(phil, "\t\t\t phil_dead 3");//
	return (1);
}

int	ft_stop(t_phil *phil)
{
	if (get_info(phil->data->dead, phil->data->died) == 1){
		just_print(phil, "\t\t\t ft_stop after info");
		return (1);
	}
	if (nowtime(phil) >= (phil->lastmeal + (phil->data->t_die / 1000)))
		return (phil_dead(phil, 1));
	// just_print(phil, "\t\t\t ft_stop after calculate if dead");
	if (get_info(phil->data->eat, phil->data->x_eat) > 0)
	{
		if (get_info(phil->data->eat, phil->data->eaten) >= phil->data->n_phil)
			return (phil_dead(phil, 0));
	}
	// just_print(phil, "\t\t\t ft_stop after every one ate x times");
	return (0);
}
int	just_print(t_phil *phil, char *act)
{
	// if (get_info(phil->data->dead, phil->data->died) == 1)
	// 	return (1);
	pthread_mutex_lock(&phil->data->w);
	printf("%ld\t%i %s\n",nowtime(phil), phil->id, act);
	pthread_mutex_unlock(&phil->data->w);
	return (get_info(phil->data->dead, phil->data->died));
}

int	write_now(t_phil *phil, char *act)
{
	if (ft_stop(phil) == 1)
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
	return (get_info(phil->data->dead, phil->data->died));
}
int	new_sleep(t_phil *phil, int time)
{
	while (nowtime(phil) < time)
	{
		if (phil->data->died != 0)
			return (1);
	}
	return(phil->data->died);
}
int	unlock_forks(t_phil *phil)
{
	pthread_mutex_unlock(phil->l_fork);
	pthread_mutex_unlock(phil->r_fork);
	return (1);
}
int	action(t_phil *phil, char *act, int time)
{
	if (ft_stop(phil) == 1)
		return (1);
	if (ft_strcomp("is eating", act) == 0)
	{
		if (write_now(phil, act) == 1){
			just_print(phil, "something happend while writing eat");
			return (unlock_forks(phil));}
		phil->lastmeal = nowtime(phil);
		if (new_sleep(phil, (time /1000) + nowtime(phil)) != 0)
			return (unlock_forks(phil));
		unlock_forks(phil);
		// just_print(phil, "\t\t\t forks unlocked");//
		phil->t_eaten++;
		return (phil->data->died);
	}
	if (write_now(phil, act) == 1)
		return (1);
	just_print(phil, "\t\t\t before sleep");//
	if (nowtime(phil) + (time / 1000) >= (phil->lastmeal + (phil->data->t_die / 1000)))
	{
		new_sleep(phil, (phil->lastmeal + (phil->data->t_die / 1000)));
		just_print(phil, "is dead");
		return (1);
	}
	return(new_sleep(phil, (time / 1000) + nowtime(phil)));
}

int	phil_ate(t_phil *phil)
{
	if (get_info(phil->data->eat, phil->data->x_eat) <= phil->t_eaten)
	{
		pthread_mutex_lock(&phil->data->eat);
		phil->data->eaten++;
		pthread_mutex_unlock(&phil->data->eat);
	}
	if (get_info(phil->data->eat, phil->data->x_eat) > 0)
	{
		if (get_info(phil->data->eat, phil->data->eaten) >= phil->data->n_phil){
			just_print(phil, "\t\t\t\t everyone ate");
			return (phil_dead(phil, 0));}
	}
	return (get_info(phil->data->dead, phil->data->died));
}
int	phil_even(t_phil *phil, int check)
{
	while (get_info(phil->data->dead, phil->data->died) == 0)
	{
		if (phil->id % 2 != 0)
		{
			if (action(phil, "is eating", phil->data->t_eat) != 0 || ft_stop(phil) != 0)
				return (1);
		}
		else
		{
			if (phil->t_eaten == 0)
				usleep(1);
			if (action(phil, "is eating", phil->data->t_eat) != 0 || ft_stop(phil) != 0)
				return (1);
		}
		if (check != 0 || ft_stop(phil) != 0)
			return (1);
		if (phil_ate(phil) != 0 || ft_stop(phil) != 0)
			return (1);
		if (action(phil, "is sleeping", phil->data->t_sleep) != 0 || ft_stop(phil) != 0)
			return (1);
		if (action(phil, "is thinking", 1) != 0 || ft_stop(phil) != 0)
			return (1);
	}
	return (0);
}

void	*routine(void *anything)
{
	t_phil	*phil;

	phil = (t_phil *)anything;
	if (phil->id == phil->data->n_phil)
	{
		gettimeofday(&phil->data->start, NULL);
		if (phil->id == 1)
		{
			usleep(phil->data->t_die);
			phil_dead(phil, 1);
			return (NULL);
		}
		pthread_mutex_unlock(&phil->data->time);
		usleep(phil->data->t_eat / 2);
	}
	else
	{
		pthread_mutex_lock(&phil->data->time);
		pthread_mutex_unlock(&phil->data->time);
	}
	phil_even(phil, 0);
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
	pthread_mutex_lock(&phil->data->time);
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
	t_data			data;


	if (argc > 4 && argc < 7)
	{
		if (create_data(&data, argc, argv) > 0)
			return (printf("invalid arguments\n"));
		data.phil = malloc((data.n_phil + 1) * sizeof(pthread_t));
		create_phils(data);
		free(data.forks);
		free(data.phil);
		return (0);
	}
	return (printf("invalid number of arguments \n"));
}
