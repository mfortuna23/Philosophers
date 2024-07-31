/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfortuna <mfortuna@student.42.pt>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 14:24:42 by mfortuna          #+#    #+#             */
/*   Updated: 2024/07/31 14:06:02 by mfortuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int	get_nbr(char *s)
{
	int		i;
	long	nbr;

	i = -1;
	nbr = 0;
	while (s[++i])
	{
		if ((s[i] == '-' || s[i] == '+') && i == 0)
		{
			if (s[i] == '-')
				return (-1);
		}
		else if (s[i] < '0' || s[i] > '9')
			return (-1);
		else
			nbr = (nbr * 10) + (s[i] - '0');
		if (nbr > 2147483647)
			return (-1);
	}
	return (nbr);
}

int	ft_get_data(t_data *data, int argc, char **argv)
{
	data->n_phil = get_nbr(argv[1]);
	if (data->n_phil < 1)
		return (-1);
	data->t_die = get_nbr(argv[2]);
	if (data->t_die < 0)
		return (-1);
	data->t_eat = get_nbr(argv[3]);
	if (data->t_eat < 0)
		return (-1);
	data->t_sleep = get_nbr(argv[4]);
	if (data->t_sleep < 0)
		return (-1);
	if (argc == 6)
	{
		data->x_eat = get_nbr(argv[5]);
		if (data->x_eat < 0)
			return (-1);
	}
	return (0);
}

void	start(t_phil **head, t_data data)
{
	pthread_t	*phill;
	t_phil		*node;
	int i;

	i = 0;
	node = (*head);
	phill = malloc((data.n_phil + 1) * sizeof(phill));
	if (!phill)
		return ;
	while (i < (node->data.n_phil))
	{
		pthread_create(&phill[i], NULL, &phil_routine, (void *)node);
		node = node->next;
	}
	i = 0;
	node = (*head);
	while (i < (node->data.n_phil))
	{
		pthread_detach(phill[i]);
		node = node->next;
	}
	free(phill);
}

// arguments:
// n of philosophers
// time to die
// time to eat
// time to sleep
// optional number of times a philosopher has to eat - if not specified, the program only stops when a philosopher dies
int	main(int argc, char **argv)
{
	t_data	data;
	t_phil	*head;
	pthread_mutex_t write_in;

	head = NULL;
	if (argc > 4 && argc < 7)
	{
		if (ft_get_data(&data, argc, argv) < 0)
			return (printf("wrong input\n"));
		pthread_mutex_init(&write_in, NULL); //
		create_struct(&head, data, &write_in);
		start(&head, data);
		
		delete_stack(&head);
		return (0);
	}
	return (printf("invalid number of arguments\n"));
}