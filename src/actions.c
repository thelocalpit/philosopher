/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfalasch <pfalasch@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 19:37:58 by pfalasch          #+#    #+#             */
/*   Updated: 2023/05/25 17:21:50 by pfalasch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosopher.h"

/* get_time

	la funzione get_time è necessaria per calcolarsi il tempo e stampare
	l'intervallo di tempo giusto quando mandiamo i messaggi ma anche quando
	dobbiamo controllare che un sia morto nessuno.
	lo facciamo con u_int64_t perchè in questo modo sappiamo per certo che
	non ci sareà nessun tipo di overflow e non abbiamo necessità dei negativi
	perchè si parla di tempo.
	la struttura timeval conterrà il tempo di sistema.
	Viene chiamata la funzione gettimeofday(&tv, NULL), che ottiene l'ora
	corrente e la memorizza nella struttura tv.
	ritorneremo il valore in MILLISECONDI calcolando
	(tv.tv_sec * 1000) + (tv.tv_usec /1000).
	Il tempo viene calcolato come la somma dei secondi convertiti
	in millisecondi (tv_sec * (u_int64_t)1000) e dei microsecondi convertiti
	in millisecondi (tv_usec / 1000). Questo calcolo tiene conto del fatto
	che ci sono 1000 millisecondi in un secondo.
	 */

u_int64_t get_time(void)
{
	struct timeval tv;

	if (gettimeofday(&tv, NULL))
	{
		printf("gettimeofday() FAILURE\n");
		// ft_free_mem(NULL);
		return (1);
	}
	return ((tv.tv_sec * (u_int64_t)1000) + (tv.tv_usec / 1000));
}

/* funzione messages.

	non so se la voglio fare. per strutturarla in maniera funzionale, dovrei
	creare condizioni secondo le quali viene scritta una cosa piuttosto
	che un'altra. Tterribi utilizza un metodo con il define nell header,
	io per ora ho deciso di printare a schermo ciascuna volta.
	infatti utilizza una funzione di lock per non far scrivere le cose
	contemporanemente ma in maniera ordianta. Molto intelligente.
	beh su qeusta cosa devo chiedere a qualcuno se c'è un'alternativa.
	però non è richiesto che venga eseguito in ordine.

	alla fine è necessario sviluppare una funzione del genere. semplifica la
	scrittura di tutta la parte del codice e utilizza la parte dei messaggi
	per definire la morte e la terminazione del programma.
	ft_strcmp serve per definire quando effettivamente il messaggio passato
	è died.
	Questa serve per mandare i messaggi a schermo.
	
	
 */


void messages(char *str, t_philo *philo)
{
	u_int64_t time;
	
	pthread_mutex_lock(&philo->data->write);
	time = get_time() - philo->data->start_time;
	if (ft_strcmp("died", str) == 0 && philo->data->dead == 0)
	{
		printf("%llu %d %s\n", time, philo->id, str);
		philo->data->dead = 1;
	}
	if (!philo->data->dead)
		printf("%llu %d %s\n", time, philo->id, str);
	pthread_mutex_unlock(&philo->data->write);
}

/* eat routine

	la funzione eat, che ha come argomento il philosofo in questione,
	è la funzione padre che racchiude tutte le micro funzioni necessarie per
	l'azione del mangiare del philo selezionato.
	quello che deve fare mangiare un filosofo è:
		1. prendere le forhcette a dx e sx. chiamo una funzione ulteriore, quella
		take forks. che semplicemente mette il lock all forchetta a dx del philo
		e a sx del philo e manda il messaggio che le ha prese.
		2. bloccare le forchette con un mutex lock. perchè le risorse sono già
		utilizzate. Questo procedimento è come andare ad agire sulla memoria
		e quindi sulle celle di memoria della forchette. perchè dobbiamo pensare
		che questo loop dell'eat sta avvenendo per tutti i philosofi. quindi
		quando un filosofo va a vedere se effettivamente c'è la forchetta,
		può trovare il "cassetto" aperto o chiuso.
		3. è necessario poi calcolare il tempo mancante alla morte perchè
		mentre mangia non può morire. sta mangiando! no? logico dai.
		ci calcoliamo quindi il tempo globale con la somma di get_time e tempo
		necessario per morire.
		4. attiviamo la variabile booleana eating che ci permette di dare
		il messaggio corretto. nella funzione message.
		5. mandiamo il messaggio a schermo che il filosofo sta mangiando.
		che è una funzione a parte. Come strutturarla?
		6. quanto dura la mangiata? beh semplice ci è data dagli argomenti,
		ovvero l'eating time. quindi daremo un tempo di attesa al processo
		che non sarà altro, se vogliamo figurarlo, che il tempo che il
		philosofo impiegherà per mangiare. ma di fatto, non succede niente a
		livello di processi! se non di bloccare le risorse.
		7. una volta finito di mangiare, riporto la variabile booleana eating
		a 0.
		8.levo il lock alle forchette;
		9. lascio le forchette con funzione drop_forks che unlocka le risorse e
		da il messaggio che sta dormendo susseguito da ft_usleep per il tempo di sleep.
 */

void	eat(t_philo *philo)
{
	// u_int64_t	time;

	pthread_mutex_lock(philo->r_fork);
	messages("has taken a fork", philo);
	// time = get_time() - philo->data->start_time;
	// printf("%llu %d has taken a fork", time, philo->id);
	pthread_mutex_lock(philo->l_fork);
	messages("has taken a fork", philo);
	// time = get_time() - philo->data->start_time;
	// printf("%llu %d has taken a fork", time, philo->id);
	pthread_mutex_lock(&philo->lock);
	philo->eating = 1;
	philo->time_to_die = get_time() + philo->data->death_time;
	messages("is eating", philo);
	// time = get_time() - philo->data->start_time;
	// printf("%llu %d is eating", time, philo->id);
	philo->eat_count++;
	ft_usleep(philo->data->eat_time);
	philo->eating = 0;
	pthread_mutex_unlock(&philo->lock);
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
	messages("is sleeping", philo);
	ft_usleep(philo->data->sleep_time);
}