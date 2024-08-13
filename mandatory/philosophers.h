/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfortuna <mfortuna@student.42.pt>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 11:20:23 by mfortuna          #+#    #+#             */
/*   Updated: 2024/08/13 13:58:29 by mfortuna         ###   ########.fr       */
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
	long int			n_phil;
	long int			t_die;
	long int			t_eat;
	long int			t_sleep;
	long int			x_eat;
	long int			eaten;
	pthread_t			*phil;
	pthread_mutex_t		w;
	pthread_mutex_t		eat;
	pthread_mutex_t		dead;
	pthread_mutex_t		time;
	struct timeval		start;
	struct timeval		now;
	int					died;
	pthread_mutex_t		*forks;
}						t_data;

typedef struct s_phil
{
	int					id;
	int					t_eaten;
	long int			lastmeal;
	pthread_mutex_t		*r_fork;
	pthread_mutex_t		*l_fork;
	struct s_phil		*next;
	t_data				*data;
}						t_phil;

void	create_struct(t_phil **head, t_data *data);
void	add_last(t_phil **head, t_data *data, int id);
t_phil	*create_node(t_data *data, int id);
void	clean_struct(t_phil **head);
void	delete_last(t_phil **head);
int		ft_strcomp(const char *s1, const char *s2);
int		ft_stop(t_phil *phil);
#endif
