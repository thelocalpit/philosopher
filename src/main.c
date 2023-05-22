/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfalasch <pfalasch@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 19:35:41 by pfalasch          #+#    #+#             */
/*   Updated: 2023/05/22 18:12:01 by pfalasch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosopher.h"

/* l'utilizzo di mutex non è altro la creazione di lucchetti. Immaginiamo
delle catene che vengono utilizzate a necessità per bloccare per un determinato
filosofo le risorse necessarie affichè possa lavorare. una volta utilizzate,
sblocchiamo quindi leviamo le catene alle risorse e lasciamo la chiave inserita,
pronte per un altro filosofo che le utilizzerà. quindi una volta che il filosofo
sta usando le risorse è come se si appropriasse della chiave per il tempo
a lui necessario a fare l'azione di mangiare. 
I mutex (o lucchetti) devono essere creati, allocati e poi distrutti una volta
che non vengono più usati. */

/* in questa funzione liberiamo la memoria allocata.
	per fare una cosa fatta a modo liberiamo tutto quello che abbiamo allocato
	o che avremmo potuto allocare.
	quindi in primis tid, poi le risorse e poi i vari filosofi (struct t_philo).
	per farlo, una forma semplice per scrivere questo comando è quello di
	mettere condizioni in modo da dire che se è stata allocata memoria (e quindi
	la variabile in questione è diversa da 0) allora facciamo il free. altrimenti
	non avremo da liberare niente e passeremo a quella successiva.
	ma prima di questo dobbiamo assicurarci anche di aver distrutto i mutex.
	per farlo possiamo usare la pthread_mutex_destroy. ricordiamochi che abbiamo
	allocato memoria per ciascuna "catena" (catena per ogni "fork")
	e per ciascun "lucchetto" (lucchetto per chiudere le "catene").
	questo significa che dobbiamo fare questa azione per ogni risorsa allocatwa.
	serve quindi un ciclo while che scorra per il numero di filosofi. 
	
	 */
void	ft_free_mem(t_data *data)
{
	int	i;

	i = -1;

	while (++i < data->philo_nb)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&data->philos[i].lock);
	}
	// ancora non ho capito a cosa servono queste variabili.
	// pthread_mutex_destroy(&data->write);
	pthread_mutex_destroy(&data->lock);
	if (data->tid != 0)
		free(data->tid);
	if (data->forks != 0)
		free(data->forks);
	if (data->philos != 0)
		free(data->philos);
}

/* in questa funzione gestiamo il caso in cui c'è un solo philosofo.
	cosa dobbiamo fare in questo caso? la forchetta sarà una sola, quindi
	il philosofo non potrà mangiare neanche.
	La funzione pthread_detach viene utilizzata per indicare al sistema che il 
	thread specificato, in questo caso data->tid[0], deve essere creato in 
	modalità detachable. Ciò significa che una volta che il thread termina 
	la sua esecuzione, le risorse associate a esso possono essere liberate 
	automaticamente dal sistema operativo, senza la necessità di chiamare 
	esplicitamente la funzione pthread_join per unire il thread.
	 */

int	ft_one_philo(t_data *data)
{
	data->start_time = get_time();
	if (pthread_create(&data->tid[0], NULL, &routine, &data->philos[0]))
	{
		printf("errore mentre vengono creati i threads");
		ft_free_mem(data);
		return (1);
	}
	pthread_detach(data->tid[0]);
	while (data->dead == 0)
		ft_usleep(0);
	ft_free_mem(data);
	return (0);
}

/* controlliamo il numero di argomenti
controlliamo con input_checker che gli argomenti abbiano i giusti input
poi inizializziamo ogni cosa iobbbono
poi caso limite di un filosofo e basta.
 */
int	main(int ac, char **av)
{
	t_data	data;

	if (!(ac == 5) || !(ac == 6))
	{
		printf("controlla gli input, qualcosa non torna!");
		return (1);
	}
	if (input_checker(av))
	{
		printf("controlla gli input, qualcosa non torna!");
		return (1);
	}
	if (ft_init(&data, ac, av))
		return (1);
	if (data.philo_nb == 1)
		return (ft_one_philo(&data));
	if (thread_init(&data))
		return (1);
	ft_free_mem(&data);
	return (0);
}
