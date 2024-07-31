/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfortuna <mfortuna@student.42.pt>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 14:13:55 by mfortuna          #+#    #+#             */
/*   Updated: 2024/07/31 14:44:27 by mfortuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int	get_number(char *nbr)
{
	int i;
	long n;

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
	data->n_phil = get_number(argv[1]);
	data->t_die = get_number(argv[2]);
	data->t_eat = get_number(argv[3]);
	data->t_sleep = get_number(argv[4]);
	if (argc == 6)
	{
		data->x_eat = get_number(argv[5]);
		if (data->x_eat < 0)
			return (-1);
	}
	if (data->n_phil < 0 || data->t_die < 0 || data->t_eat < 0 || data->t_sleep < 0)
		return (-1);
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;
	if (argc > 4 && argc < 7)
	{
		if (create_data(&data, argc, argv) < 0)
			return (-1);
		printf("n_phill : %i\nt_die : %i\nt_eat : %i\nt_sleep : %i\n", data.n_phil, data.t_die, data.t_eat, data.t_sleep);
		return (0);
	}
	printf("invalid number of arguments \n");
}