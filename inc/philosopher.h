/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfalasch <pfalasch@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 18:19:51 by pfalasch          #+#    #+#             */
/*   Updated: 2023/05/09 17:27:36 by pfalasch         ###   ########.fr       */
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

/* la struttura philo raccoglie tutte le variabili necessarie per definire le
	caratteristiche di ciascun filosofo. 
	troviamo il t1 che è un valore necessario per definire il thread.
	??? id:
	eat count: tiene conto del numero di volte che il filosofo ha mangiato.
	status: serve per sapere in che status è il filosofo (thinking, eating,
	sleeping).
	eating: è necesario per apere se sta ancora mangiando. puô capitare infatti
	che il filosofo arrivi al punto in cui deve morire, ma invece lo lasciamo vivo
	perchè ha già cominciato il processo di mangiare, quindi è riuscito a sopravviere
	??? time to die: necessario sapere quanto tempo manca a morire.
	mutex: servono per bloccare le risorse per quel determinato filosofo, fintanto
	che non avrà finito di mangiare. questo permetterà di non avere problemi di race.
	lock, r_fork, l_fork.
	 */
	
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

/* la struttura data serve per raccogliere le variabili provenineti dagli argomenti passati.
	il puntatore tid, serve per identificare il numero di ciascun thread crreato, quindi il 
	numero di riconoscimento del philosopher in sostanza. 
	phil num: è il numero di filosofi totali
	meal_nb:  è il numero di pasti che ogni filosofo deve fare.
	dead: variabile booleana che ci serve per capire se c'è qualche morto o meno.
	stop: se c'è qualcuno che ha finito e quanti hanno finito.
	dead, eat, sleep, start time: variabili raccolte dagli argomenti. 
	??? mutex: forks, lock, write: da capire bene come e perchè. */
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

/* NB. ogni struttura chiama láltra. possiamo quindi andare a modificare anche i dati della struttura 
chiamata all'	interno. */

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