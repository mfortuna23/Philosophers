/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfortuna <mfortuna@student.42.pt>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 11:20:23 by mfortuna          #+#    #+#             */
/*   Updated: 2024/08/07 15:56:29 by mfortuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <errno.h>
# include <stdbool.h>

typedef struct s_data
{
	int				n_phil;
	int				t_die;
	int				t_eat;
	int				t_sleep;
	int				x_eat;
	int				eaten;
	pthread_t		*phil;
	pthread_mutex_t	w;
	pthread_mutex_t	eat;
	pthread_mutex_t	dead;
	int				died;
	pthread_mutex_t	*forks;
}					t_data;

typedef struct s_phil
{
	int				id;
	struct s_phil	*next;
	t_data			*data;
}					t_phil;

void	create_struct(t_phil **head, t_data *data);
void	add_last(t_phil **head, t_data *data, int id);
t_phil	*create_node(t_data *data, int id);
void	clean_struct(t_phil **head);
void	delete_last(t_phil **head);

#endif
