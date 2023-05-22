/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfalasch <pfalasch@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 19:38:06 by pfalasch          #+#    #+#             */
/*   Updated: 2023/05/22 16:38:32 by pfalasch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../inc/philosopher.h"

/* funzione messages.
	
	non so se la voglio fare. per strutturarla in maniera funzionale, dovrei
	creare condizioni secondo le quali viene scritta una cosa piuttosto
	che un'altra. Tterribi utilizza un metodo con il define nell header,
	io per ora ho deciso di printare a schermo ciascuna volta. 
	infatti utilizza una funzione di lock per non far scrivere le cose
	contemporanemente ma in maniera ordianta. Molto intelligente.
	beh su qeusta cosa devo chiedere a qualcuno se c'è un'alternativa. 
	però non è richiesto che venga eseguito in ordine.  */


/* monitor
	
	la funzione di monitor ci serve invece solo per controllare se il processo
	è terminato, ovvero se stop ha raggiunto il numero uguale 
	al totale dei filosofi.
	viene fatto un ciclo che va avanti fintanto che dead è a 0. questo ciclo 
	controlla se effettivamente tutti hanno finito.
	la condizione che verifica la cosa è se stop ha pareggiato o superato il
	nuemro di philos.
	a quel punto la variabile dead sarà uguale a 1 e uscirà dal loop. Non perchè
	è morto qualcuno ma perchè ci evitiamo di fare un'altra variabile.
	 */

void	*monitor(void *philo_p)
{
	t_philo	*philo;

	philo = (t_philo *)philo_p;
	// printf("data val: %d", philo->data->dead);
	while (philo->data->dead == 0)
	{
		pthread_mutex_lock(&philo->lock);
		if (philo->data->stop >= philo->data->philo_nb)
			philo->data->dead = 1;
		pthread_mutex_unlock(&philo->lock);
	}
	return ((void *)0);
}

/* supervisor
	
	funzione necessaria pe controllare se un philo è morto oppure no
	come anche routine, questa funzione prendere come argomento un puntatore
	void e ritorna un puntatore void
	stesso giochetto, facciamo il casting al type t_philo.
	qui se il tempo calcolato con get_time è uguale o maggiore del tempo di
	e il philo non sta a magnà, allora si manda il messaggio che è morto.
	la questione realtiva al lock della struttura philo e al lock della
	struttura data  è fondamentale: con il lock alla struttura philo ci assicuriamo
	che non avvenga una race di nessun tipo sulla struttura e stesso su quella
	di data nel momento in cui gestiamo la parte relativa al termine del 
	compito del philo, ovvero quella di mangiare tot volte. mettendo il lock,
	prima di far andare avanti gli altri processi (ricordiamoci che il thread
	continua ad andare a diritto senza tregua) ci assicuriamo di controllare
	se il philo ha ha concluso il suo numero di pasti e se tutti i philos hanno
	finito di mangiare. in questo modo, con le giuste condizioni non facciamo andare
	avanti i philos.  
	
	Non capisco perchè aumento anche l'eat count quando eat count è uguale a meals nb.
	 */

void	*supervisor(void *philo_p)
{
	t_philo	*philo;

	philo = (t_philo *)philo_p;
	while (philo->data->dead == 0)
	{
		pthread_mutex_lock(&philo->lock);
		if (get_time() >= philo->time_to_die && philo->eating == 0)
		{
			time = get_time() - philo->data->start_time;
			printf("%llu %d philo is dead", time, philo->id);
		}
		if (philo->eat_count == philo->data->meals_nb)
		{
			pthread_mutex_lock(&philo->data->lock);
			philo->data->stop++;
			philo->eat_count++;
			pthread_mutex_unlock(&philo->data->lock);
		}
	}
	pthread_mutex_unlock(&philo->lock);
}

/* routine
		nella funzione routine creeremo anche il thread di supervisor.
	nella struttura di t_philos abbiamo già un thread ad hoc (t_super) che dedichiamo al
	supervisor di ciascun filosofo.

	subito dobbiamo gestire la questione dell'argomento puntatore void:
	lo castiamo a una struct t_philo.
	Perchè castiamo il pointer? semplicemente per una questione di leggibilità
	del codice.
	la cosa importante è che dichiariamo una struttura philo per indicare il
	philosofo in questione in quel thread.

	subito dopo la cosa importante da fare è gestire il tempo di morte dei philos.
	per farlo dobbiamo torvare la somma fra il tempo di sistema attuale (get_time)
	e il tempo di morte.
	Attiveremo poi la funzione di supervisor. questa come abbiamo già visto,
	è necessaria per controllare attreaverso un'altro thread che creiamo come sta
	messo a tempo dalla morte (poaraccio).

	poi quello che faremo è creare una condizione booleana affinchè il loop
	continui fintanto che il philo è vivo.
	se entra in questo loop, avvieremo la funzione eat e subito dopo che
	finisce di mangiare entra invece passa in thinking e a quel punto va avanti
	finchè qualcuno non muore.
	dopo è necessario che si controlli che il processo del supervisor sia
	concluso con un pthread join.
	 */

void	*routine(void *philo_pointer)
{
	t_philo	*philo;

	philo = (t_philo *) philo_pointer;
	philo->time_to_die = get_time() + philo->data->death_time;
	if (pthread_create(philo->t_super, NULL, &supervisor, (void *) philo))
		return ((void *)1);
	while (philo->data->dead == 0)
	{
		eat(philo);
		time = get_time() - philo->data->start_time;
		printf("%llu %d philo is thinking", time, philo->id);
	}
	if (pthread_join (philo->t_super, NULL))
		return ((void *)1);
	return ((void *)0);
}


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
			ft_free_mem(data);
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
			ft_free_mem(data);
			return (1);
		}
	}
	return (0);
}
