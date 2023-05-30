/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfalasch <pfalasch@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 18:19:51 by pfalasch          #+#    #+#             */
/*   Updated: 2023/05/30 17:34:36 by pfalasch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H
# include <stdbool.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>

struct	s_data;
typedef struct s_philo
{
	struct s_data	*data;
	pthread_t		t_super;
	int				id;
	int				eat_count;
	int				eating;
	uint64_t		time_to_die;
	pthread_mutex_t	lock;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
}		t_philo;

typedef struct s_data
{
	pthread_t		*tid;
	int				philo_nb;
	int				meals_nb;
	int				dead;
	int				stop;
	t_philo			*philos;
	u_int64_t		death_time;
	u_int64_t		eat_time;
	u_int64_t		sleep_time;
	u_int64_t		start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	lock;
	pthread_mutex_t	write;
}		t_data;

int			ft_free_mem(t_data *data);
u_int64_t	get_time(void);
void		eat(t_philo *philo);
int			ft_alloc_mem(t_data *data);
int			ft_init_data(t_data *data, int ac, char **av);
int			ft_init_forks(t_data *data);
void		ft_init_philo(t_data *data);
void		*monitor(void *philo_p);
void		*supervisor(void *philo_p);
void		*routine(void *philo_pointer);
int			thread_init(t_data *data);
int			input_checker(char **av);
int			ft_usleep(useconds_t time);
long		ft_atoi(char *str);
int			ft_init(t_data *data, int ac, char **av);
int			ft_strcmp(char *s1, char *s2);
void		messages(char *str, t_philo *philo);
int			ft_thread_routine(t_data *data);
int			ft_thread_mon(t_data *data);
int			ft_thread_join(t_data *data);

#endif