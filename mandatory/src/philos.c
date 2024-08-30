/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfortuna <mfortuna@student.42.pt>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 15:04:22 by mfortuna          #+#    #+#             */
/*   Updated: 2024/08/30 15:38:26 by mfortuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

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
			return (unlock_forks(phil));
		if (just_print(phil, act) == 1)
			return (1);
		return (0);
	}
	else if (just_print(phil, act) == 1)
		return (1);
	return (0);
}

int	action(t_phil *phil, char *act, long int time)
{
	if (get_info(phil, SIM) == 1)
		return (1);
	if (ft_strcomp("is eating", act) == 0)
	{
		if (write_now(phil, act) == 1)
			return (1);
		if (update_meal(phil, time) == 1)
			return (1);
		phil->t_eaten++;
		if (phil->t_eaten == phil->data->x_eat)
			change_data(phil, EAT);
		return (get_info(phil, SIM));
	}
	if (write_now(phil, act) == 1)
		return (1);
	if (nowtime(phil->data->start) + (time / 1000) >= \
	(phil->lastmeal + (phil->data->t_die / 1000)))
	{
		if (new_sleep(phil, (phil->lastmeal + (phil->data->t_die / 1000))) == 1)
			return (1);
		return (change_data(phil, DIED));
	}
	return (new_sleep(phil, (time / 1000) + nowtime(phil->data->start)));
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
		if (action(phil, "is thinking", 1000) != 0)
			return (1);
	}
	return (0);
}

void	*routine(void *anything)
{
	t_phil	*phil;

	phil = (t_phil *)anything;
	while (get_info(phil, SIM) != 0)
		usleep(500);
	if (phil->id == phil->data->n_phil)
	{
		if (phil->id == 1)
		{
			just_print(phil, "has taken a fork");
			usleep(phil->data->t_die);
			change_data(phil, DIED);
			return (NULL);
		}
		if (phil->data->n_phil % 2 != 0)
			usleep(phil->data->t_eat / 2);
	}
	phil_even(phil, 0);
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
