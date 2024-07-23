/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfortuna <mfortuna@student.42.pt>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 14:24:42 by mfortuna          #+#    #+#             */
/*   Updated: 2024/07/23 14:47:26 by mfortuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

ft_get_data(t_phil *data, int argc, char **argv)
{
	
}

// arguments:
// n of philosophers
// time to die
// time to eat
// time to sleep
// optional number of times a philosopher has to eat - if not specified, the program only stops when a philosopher dies
int	main (int argc, char **argv)
{
	t_phil	data;
	
	if (argc > 4 && argc < 7)
	{
		ft_get_data(&data, argc, argv);
	}
	return (printf("invalid number of arguments\n"));
}