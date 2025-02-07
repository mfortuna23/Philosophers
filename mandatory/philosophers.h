/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfortuna <mfortuna@student.42.pt>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 11:26:12 by mfortuna          #+#    #+#             */
/*   Updated: 2024/08/30 15:38:06 by mfortuna         ###   ########.fr       */
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

typedef enum e_op
{
	SIM,
	EAT,
	DIED,
}			t_op;

typedef struct s_data
{
	long int			n_phil;
	long int			t_die;
	long int			t_eat;
	long int			t_sleep;
	long int			x_eat;
	long int			x_eaten;
	pthread_t			*phil;
	pthread_mutex_t		w;
	pthread_mutex_t		eat;
	pthread_mutex_t		dead;
	pthread_mutex_t		time;
	struct timeval		start;
	struct timeval		now;
	int					sim;
	pthread_mutex_t		*forks;
	struct s_phil		*head;
}						t_data;

typedef struct s_phil
{
	int					id;
	int					t_eaten;
	long int			lastmeal;
	pthread_mutex_t		*r_fork;
	pthread_mutex_t		*l_fork;
	struct s_phil		*next;
	int					dead;
	int					full;
	struct s_data		*data;
}						t_phil;

void		create_struct(t_phil **head, t_data *data);
void		add_last(t_phil **head, t_data *data, int id);
t_phil		*create_node(t_data *data, int id);
void		clean_struct(t_phil **head);
void		delete_last(t_phil **head);
int			ft_strcomp(const char *s1, const char *s2);
int			ft_stop(t_phil *phil);
int			create_data(t_data *data, int argc, char **argv);
int			mutex_create(t_data *data);
long int	get_number(char *nbr);
int			get_info(t_phil *phil, t_op code);
int			unlock_forks(t_phil *phil);
int			change_data(t_phil	*phil, t_op code);
void		*phil_dead(t_phil *phil);
long int	last_meal(t_phil *phil);
void		*check_pthread(void *anything);
void		create_phils(t_data *data);
int			phil_even(t_phil *phil, int check);
void		*routine(void *anything);
int			action(t_phil *phil, char *act, long time);
int			write_now(t_phil *phil, char *act);
long int	nowtime(struct timeval start);
int			just_print(t_phil *phil, char *act);
int			new_sleep(t_phil *phil, int time);
int			just_print(t_phil *phil, char *act);
long int	nowtime(struct timeval start);
int			sim_continue(t_phil *current, t_data *data);
int			update_meal(t_phil *phil, long int time);
#endif
