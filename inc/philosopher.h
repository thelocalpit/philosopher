/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfalasch <pfalasch@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 18:19:51 by pfalasch          #+#    #+#             */
/*   Updated: 2023/05/08 19:34:04 by pfalasch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	PHILO_H
# define PHILO_H
# include <stdbool.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>

struct s_data;

typedef struct s_philo
{
	struct			s_data *data;
	pthread_t		t1;
	int 			id;
	int				eat_count;
	int 			status;
	int		 		eating;
	uint64_t 		time_to_die;
	pthread_mutex_t lock;
	pthread_mutex_t r_fork;
	pthread_mutex_t l_fork;
}		t_philo;

typedef struct	s_data
{
	pthread_t		*tid;
	int				philo_num;
	int				meals_nb;
	int				dead;
	int 			stop;
	t_philo 		*philos;
	u_int64_t		death_time;
	u_int64_t		eat_time;
	u_int64_t		sleep_time;
	u_int64_t		start_time;
	pthread_mutex_t *forks;
	pthread_mutex_t lock;
	pthread_mutex_t write;
} 		t_data;

//	input_err
# define ERR_IN_1 "INVALID INPUT CHARACTER"
# define ERR_IN_2 "INVALID INPUT VALUES"

//	pthread_err
# define TH_ERR "ERROR WHILE CREATING THREADS"
# define JOIN_ERR "ERROR WHILE JOINING THREADS"
# define INIT_ERR_1 "ERROR WHILE INIT FORKS"

//	time_err WTFFFF
# define TIME_ERR "UNABLE TO RETRIVE UTC"

//	philo_msg
# define TAKE_FORKS "has taken a fork"
# define THINKING "is thinking"
# define SLEEPING "is sleeping"
# define EATING "is eating"
# define DIED "died"

#endif