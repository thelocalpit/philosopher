/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfalasch <pfalasch@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 19:35:41 by pfalasch          #+#    #+#             */
/*   Updated: 2023/05/10 18:09:17 by pfalasch         ###   ########.fr       */
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
	int i;

	i = -1;

	while (++i < data->philo_nb)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&data->philos[i].lock);
	}
	/* ancora non ho capito a cosa servono queste variabili. */
	// pthread_mutex_destroy(&data->write);
	// pthread_mutex_destroy(&data->lock);
	if (data->tid != 0)
		free(data->tid);
	if (data->forks != 0)
		free(data->forks);
	if (data->philos != 0)
		free(data->philos);
}
/* in questa funzione vogliamo allocare la memoria necessaria, in primis per i
threads che creeremo "TID" (uguale al numero di filosofi, perchè in realtà
ciascun filosofo è un thread), poi allochiamo la memoria per le forchette ed
infine allochiamo la memoria per ciascun philosofo e ogni sua variabile
necessaria che abbiamo opportunamente inserito all'interno della struttura.
 dubbi? si, io ho dubbi sull'allocazione delle forks. perchè le alloco come
 mutex???
 ogni volta devo controllare se la memoria è stata allocata bene.
 e qui entra in gioco una funzione che creiamo che andrà a liberare la memoria
 che potremmo aver già occupato. e
 sempio: se ho allocato memoria per tid correttamente, e invece sbaglia ad
 allocare le risorse (forks) dobbiamo comunque liberare anche tid.
 quindi ci sarà la chiamata ad una funzione esterna che mi farà questo piccolo
 lavoretto easy easy.

 */
int	ft_alloc_mem(t_data *data)
{
	data->tid = malloc(sizeof(pthread_t) * data->philo_nb);
	if (data->tid)
	{
		ft_free_mem(data);
		printf("errore nell'allocazione dei threadssss");
		return (1);
	}
	data->forks = malloc(sizeof(pthread_t) * data->philo_nb);
	if (data->forks)
	{
		ft_free_mem(data);
		printf("errore nell'allocazione delle forchette diobbbono");
		return (1);
	}
	data->philos = malloc(sizeof(t_philo) * data->philo_nb);
	if (data->philos)
	{
		ft_free_mem(data);
		printf("errore nell'allocazione dei filosofi per Diana!");
		return (1);
	}
	return (0);
}
/* per tradurre gli argomenti da testo a digit utilizziamo atoi
ci passiamo tutti gli argomenti nella struttura data
siccome possono essere sia 5 che 6 gli argomenti, dobbiamo gestire il numero
di pasti se specificati
se ac == 5, allora il meal_nb lo metto in negativo in modo che non nel
conteggio che verrà fatto non inficierà sulle funzioni successive e non
si fermerà
controlliamo poi che gli argomenti inseriti siano validi per far funzionare
il programma
la variabile dead (e finished) è inizializata a 0 perchè ci servirà più 
avanti per fermare il programma in caso di una morte
le ultime due chiamate a funzioni sono per inizializzare write e lock, anche queste
variabili all'interno della struttura.
La variabile "write" è utilizzata per garantire che la scrittura su stdout 
sia eseguita in modo sincronizzato dai vari thread, 
mentre la variabile "lock" è utilizzata per proteggere l'accesso a risorse 
condivise. */

int	ft_init_data(t_data *data, int ac, int **av)
{
	data->philo_nb = (int)ft_atoi(av[1]);
	data->death_time = (u_int64_t)ft_atoi(av[2]);
	data->eat_time = (u_int64_t)ft_atoi(av[3]);
	data->sleep_time = (u_int64_t)ft_atoi(av[4]);
	if (ac == 6)
		data->meals_nb = (int)ft_atoi(av[5]);
	else
		data->meals_nb = -1;
	if (data->philo_nb <= 0 || data->philo_nb >= 200 || data->death_time < 0
		|| data->eat_time < 0 || data->sleep_time < 0)
	{
		printf("uno degli argomento ha un valore errato\n");
		printf("prendi spunto dal seguente: 5 800 200 200 7\n");
		return (1);
	}
	data->dead = 0;
	data->stop = 0;
	// pthread_mutex_init(&data->write, NULL);
	// pthread_mutex_init(&data->lock, NULL);
}

/* questa funzione è concettualmente la più complicata da capire per me.
	sostanzialmente dobbiamo inizializzare le risorse.
	e per farlo come facciamo? allochiamo memoria per tante forchette quanti
	sono i filosofi. 
	Per fare questa cosa, utilizziamo pthread_mutex_init, inizializzando lárray di
	fork che abbiamo dichiarato nella struttura data.
	in questo modo siamo in grado di utilizzare per ciascuna forchetta lock, unlock.
	Non solo: in questa funzione cominciamo ad associare 
	le prime forchette al primo filosofo.
	Successivamente utilizzo due array che mi permettano di inizializzare 
	sia le forchette a dx che quelle a sx. in questo modo avrò già 
	a disposizione in ordine corretto le forchette di dx e di sx.
	significa che alla forchetta di in posizione X di ciascun array corrisponderà 
	quella a dx e a sx del philo X. questa non è una cosa necessaria, 
	ma risulterà utile e ci permetterà di semplificare il codice e renderlo piû 
	comprensibile invece che fare le assegnazioni ogni volta.
	quindi esempio: considerando il philo n.0, 
	la forchetta a dx sarà la 0, e quella a sx sarà la 0. 
	 */
int	ft_init_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_nb)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
	i = 1;
	while (i < data->philo_nb)
	{
		data->philos[i].r_fork = &data->forks[i - 1];
		data->philos[i].l_fork = &data->forks[i];
	}
	data->philos[0].r_fork = &data->forks[data->philo_nb - 1];
	data->philos[0].r_fork = &data->forks[0];
	return (0);
}

/* questa funzione inizializza le variabili di ciascun philo. 
	in questo modo ci prepariamo definitivamente al loop definitivo.
	daje */
void	ft_init_philo(t_data	*data)
{
	int	i;

	i = -1;
	while (++i < data->philo_nb)
	{
		data->philos[i].data = data;
		data->philos[i].id = i;
		data->philos[i].eat_count = 0;
		data->philos[i].eating = 0;
		data->philos[i].status = 0;
		data->philos[i].time_to_die = data->death_time;
		pthread_mutex_init(&data->philos[i].lock, NULL);
		i++;
	}
}

/* 	questa funzione mi serve per inizializzare tutti i valori delle struct e per
	eseguire i check del caso per gli input
	in questa funzione diamo valore a tutte le variabili necessarie 
	nella struttura data.
	successivamente allochiamo la memoria ft_alloc_mem per threads, forks, e philos.
	poi what??
	poi dobbiamo inizializzare le risorse, ovvero le forks. 
	per farlo quindi dobbiamo fare una funzione dove inizializziamo con il mutex 
	tutte le forchette. per maggiori specifiche guarda la funzione.
	da ultimo dobbiamo inizializzare i philosofi.
	
	 */

	int ft_init(t_data *data, int ac, int **av)
{
	if (ft_init_data(data, av, ac))
		return (1);
	if (ft_alloc_mem(data))
		return (1);
	if (ft_init_forks(data))
		return (1);
}

// Controllo subito con questa funzione che i caratteri inseriti siano giusti
int	input_checker(char **av)
{
	int	i;
	int	j;

	i = 1;
	while (av[i])
	{
		j = 0;
		while (av[i][j])
		{
			if (av[i][j] == ' ')
				j++;
			else if (!(av[i][j] >= '0' && av[i][j] <= '9'))
			{
				printf("controlla gli input, qualcosa non torna!");
				return (1);
			}
		j++;
		}
	i++;
	}
	return (0);
}

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
	ft_init_philos(data);