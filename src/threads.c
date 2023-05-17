/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfalasch <pfalasch@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 19:38:06 by pfalasch          #+#    #+#             */
/*   Updated: 2023/05/17 18:16:15 by pfalasch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../inc/philosopher.h"

/* in questo foglio andremo a creare il nostro fantastico loop continuo
per mettere in azione i filosofi. il loop agirà sia sulle azioni da fare ma
anche su i controlli. 
In pratica quello che facciamo in questa prima funzione init, è quella di creare
e avviare i thread che andranno avanti finchè non diremo di smettere.
Quando creiamo un thread questo, può non fare niente (ma è inutile) ma può
invece chiamare una funzione in questo caso routine che farà svolgere
in maniera continua un processo a ciascun thread creato. 
immaginiamoci come se facessi partire più tab di una pagina di browser.
ognuna caricherà e farà il suo processo. solo che noi andremo anche 
a gestire come tutte qeuste si comporteranno fra loro, perchè ipotizzando
che serva a ciascuna pagina accedere a internet e solo alcune per volta 
possono farlo perchè sennò si blocca tutto, dovremo gestire gli accessi
di queste in modo che ci sia un cazzo di ordine. 
quindi routine diremo "hey amico te pupoi andare" mentre diremo anche 
"tu stai cazzo fermo deficiente, non lo vedi che sennò blocchi tutto?".

 */

/* questa prima funzione viene chiamata dal main.
	a cosa serve?
	dichiariamo un int e un nuovo thread. Questo nuovo thread è
	quello del monitor.
	Ci serve perchè qui chiameremo due volte la funzione pthread_create:
	1. per avviare il thread di monitor che andrà costantemente a
	controllare che tutto stia filando liscio.
	2. in un while che ci permetterà di procedere con la creazione di tanti threads
	quanti sono i filosofi.
	Una volta creati, questi thread continueranno a funzionare finchè avranno la
	possibilità. i termini secondo i quali si devono fermare sono specificati
	nella funzione che viene chiamata Routine.
	pthread_create passa come argomenti anche funzione che serve per dire cosa deve
	fare il thread una volta creato. ritorna un puntato void e chiede
	un puntatore void. 

	quindi come gestiamo questo argomento void?
	semplicemente lo castiamo a un t_philo che nel nostro header è una struct con
	tutto il necessario per definire il philo in questione. 

	quindi pthread_join serve per dare ordine e aspettare che un determinato thread 
	(che passiamo come argomento) terminato. necessario come in questo caso che 
	ci sarà un momento (la morte) che determinerà la fine del thread.
	
	la chiamata pthread_join(data->tid[i], NULL) viene utilizzata per
	attendere la terminazione di ciascun thread creato precedentemente
	con pthread_create.
	Questo assicura che il thread chiamante sospenda la propria esecuzione
	e attenda fino a quando il thread specificato da data->tid[i]
	ha completato la sua esecuzione.
	La funzione pthread_join blocca l'esecuzione del thread chiamante fino a 
	quando il thread specificato termina. Una volta che il thread termina, 
	il controllo ritorna al thread chiamante e può procedere 
	con le successive istruzioni.
	con il ciclo dove chiamiamo phthread_join, controlliamo che i processi siano finiti
	e si può proseguire con gli altri.
	

	utilizziamo la funzione get_time per calcolare il timestamp (in int64)
	in millisecondi per definire l'inizio del processo.

	ft_usleep necessario per creare tempo d'attesa. 
	Rifacciamo la funzione per i fatti nostri perchè è la funzione standard usleep 
	è stata deprecata.
	

	nella funzione routine creeremo anche il thread di supervisor. 
	nella struttura di t_philos abbiamo già un thread ad hoc (t1) che dedichiamo al 
	supervisor di ciascun filosofo.
	
	 */

int	thread_init(t_data *data)
{
	int			i;
	pthread_t	t_mon;

	data->start_time = get_time();
	if (data->meals_nb > 0)
	{
		if (pthread_create(&t_mon, NULL, &routine, &data->philos[i]))
		{
			printf("errore mentre vengono creati i threads");
			ft_free_mem(data);
			return (1);
		}
	}
	i = -1;
	while (++i < data->philo_nb)
	{
		if (pthread_create(&data->tid[i], NULL, &routine, &data->philos[i]))
		{
			printf("errore: senza threads corretti un si lallera");
			return (1);
		}
		ft_usleep(1);
	}
	i = -1;
	while (++i < data->philo_nb)
	{
		if (pthread_join(data->tid[i], NULL) != 0)
		{
			printf("hello hello qui un si joina a modino, correggere grazie!");
			return (1);
		}
	}
	return (0);
}
