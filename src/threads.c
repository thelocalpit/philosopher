/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfalasch <pfalasch@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 19:38:06 by pfalasch          #+#    #+#             */
/*   Updated: 2023/05/19 19:38:52 by pfalasch         ###   ########.fr       */
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
		printf("philo is thinking"); 
		/* questo printf deve però contenere la stampa anche del tempo in cui 
		viene stampato, l'id del philosofo e il messaggio. per ora ho messo 
		solo il messaggio */
	}
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
