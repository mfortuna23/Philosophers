/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfortuna <mfortuna@student.42.pt>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 11:20:23 by mfortuna          #+#    #+#             */
/*   Updated: 2024/07/30 11:45:32 by mfortuna         ###   ########.fr       */
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
	int			n_phil;
	int			t_die;
	int			t_eat;
	int			t_sleep;
	int			x_eat;
}				t_data;

typedef struct s_phil
{
	struct s_phil		*prev;
	struct s_phil		*next;
	t_data				data;
	int					id;
	bool				left_fork;
	bool				rigth_fork;
	bool				eat;
	bool				sleep;
	bool				think;
}						t_phil;

int		ft_get_data(t_data *data, int argc, char **argv);
int		get_nbr(char *s);
void	create_struct(t_phil **head, t_data data);
void	add_node(t_phil **head, int nbr, t_data data);
t_phil	*create_node(int nbr, t_data data);
void	delete_node(t_phil **head);
void	delete_stack(t_phil **head);
void	*phil_routine(void *node);

#endif
