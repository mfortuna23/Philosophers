/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfortuna <mfortuna@student.42.pt>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 11:38:09 by mfortuna          #+#    #+#             */
/*   Updated: 2024/08/30 15:33:41 by mfortuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

long int	nowtime(struct timeval start)
{
	long int		seconds;
	long int		micro;
	long int		mil;
	struct timeval	now;

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
	printf("%ld\t%i %s\n", nowtime(phil->data->start), phil->id, act);
	pthread_mutex_unlock(&phil->data->w);
	return (get_info(phil, SIM));
}

int	new_sleep(t_phil *phil, int time)
{
	while (nowtime(phil->data->start) < time)
	{
		if ((phil->lastmeal + (phil->data->t_die / 1000)) <= \
		nowtime(phil->data->start))
			return (change_data(phil, DIED));
		if (get_info(phil, SIM) != 0)
			return (1);
		usleep(500);
	}
	return (get_info(phil, SIM));
}

int	ft_strcomp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return (-1);
	while (s1[i] == s2[i] && s1[i])
		i++;
	if (s1[i] != 0 || s2[i] != 0)
		return (-1);
	return (0);
}

void	clean_struct(t_phil **head)
{
	while ((*head)->next != NULL)
		delete_last(head);
	free(*head);
	return ;
}
