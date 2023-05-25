/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfalasch <pfalasch@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 18:19:51 by pfalasch          #+#    #+#             */
/*   Updated: 2023/05/25 17:43:50 by pfalasch         ###   ########.fr       */
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

/* la struttura philo raccoglie tutte le variabili necessarie per definire le
	caratteristiche di ciascun filosofo. 
	troviamo il t1 che è un valore necessario per definire il thread.
	??? id:
	eat count: tiene conto del numero di volte che il filosofo ha mangiato.
	status: serve per sapere in che status è il filosofo (thinking, eating,
	sleeping).
	eating: è necesario per apere se sta ancora mangiando. puô capitare infatti
	che il filosofo arrivi al punto in cui deve morire, ma invece lo 
	lasciamo vivo perchè ha già cominciato il processo di mangiare, 
	quindi è riuscito a sopravviere
	??? time to die: necessario sapere quanto tempo manca a morire.
	mutex: servono per bloccare le risorse per quel determinato filosofo, 
	fintanto che non avrà finito di mangiare. questo permetterà 
	di non avere problemi di race.
	lock, r_fork, l_fork.
	 */

typedef struct s_philo
{
	struct s_data	*data;
	pthread_t		t_super;
	int				id;
	int				eat_count;
	// int				status;
	int				eating;
	uint64_t		time_to_die;
	pthread_mutex_t	lock;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
}		t_philo;

/* la struttura data serve per raccogliere le variabili provenineti 
	dagli argomenti passati.
	il puntatore tid, serve per identificare il numero di ciascun 
	thread crreato, quindi il numero di riconoscimento del philosopher
	in sostanza. 
	phil num: è il numero di filosofi totali
	meal_nb:  è il numero di pasti che ogni filosofo deve fare.
	dead: variabile booleana che ci serve per capire se c'è qualche morto o meno.
	stop: se c'è qualcuno che ha finito e quanti hanno finito.
	dead, eat, sleep, start time: variabili raccolte dagli argomenti. 
	??? mutex: forks, lock, write: da capire bene come e perchè. */
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

#endif