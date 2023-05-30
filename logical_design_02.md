MAIN

/* controlliamo il numero di argomenti
controlliamo con input_checker che gli argomenti abbiano i giusti input
poi inizializziamo ogni cosa iobbbono
poi caso limite di un filosofo e basta.
 */

FREE_MEM

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

ONE PHILO

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


-------------------------------------------------------------------------------------------------

ACTIONS.C 


GET TIME

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

MESSAGES

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

 EAT ROUTINE 

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


---------------------------------------------------------------------------------------------

INIT.C

FT_INIT

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

FT_INIT_DATA

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
le ultime due chiamate a funzioni sono per inizializzare write e lock,
anche queste variabili all'interno della struttura.
La variabile "write" è utilizzata per garantire che la scrittura su stdout
sia eseguita in modo sincronizzato dai vari thread,
mentre la variabile "lock" è utilizzata per proteggere l'accesso a risorse
condivise. */

FT_INIT_PHILO

/* questa funzione inizializza le variabili di ciascun philo.
in questo modo ci prepariamo definitivamente al loop definitivo.
daje */

FT_INIT_FORKS

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

FT_ALLOC_MEM

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

 -------------------------------------------------------------------------------------------

THREAD.C

 THREAD_INIT

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
	è stata deprecata e la rendiamo più veloce.
	

	pthread_join wich is simply going to tell the program "wait untill all the 
	threads terminate in order to continue the execution of the program".


	 */


ROUTINE


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

SUPERVISOR

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


MONITOR

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

-------------------------------------------------------------------------------------------

UTILS.C

FT_USLEEP

/* Nella riga di codice usleep(time / 10), viene utilizzata la divisione per 10
del valore time passato come parametro alla funzione ft_usleep.Questa divisione
viene eseguita per ridurre il tempo di sospensione effettivo durante ciascuna
iterazione del ciclo while.La funzione usleep viene utilizzata per sospendere
l'esecuzione del programma per un determinato periodo di tempo espresso
in microsecondi. Tuttavia, in alcuni sistemi operativi o configurazioni,
la precisione di usleep potrebbe essere limitata a intervalli di tempo
più grandi, ad esempio millisecondi. Dividendo time per 10, si riduce il tempo
di sospensione effettivo a un decimo del valore originale.Questo viene fatto per
garantire che il ciclo while venga eseguito più volte, consentendo una maggiore
precisione nella misurazione del tempo trascorso.Ad esempio, se il parametro time
viene impostato su 100.000 microsecondi(0, 1 secondi), la funzione usleep verrà
chiamata con l'argomento 10.000 microsecondi (0,01 secondi) durante ciascuna
iterazione del ciclo while. Ciò consente di avere un controllo più preciso
sul tempo trascorso e assicura che il tempo totale di sospensione sia vicino
al valore desiderato.

il millisecondo (ms), che rappresenta un millesimo di secondo (0,001 secondi), 
il microsecondo (µs), che rappresenta un milionesimo di secondo (0,000001 secondi). */


INPUT CHECKER

/* La funzione get_time restituisce il tempo attuale in millisecondi 
come un valore di tipo u_int64_t. Utilizza la funzione di sistema 
gettimeofday per ottenere il tempo di sistema corrente.

La funzione gettimeofday richiede due argomenti:

Il primo argomento è un puntatore a una struttura timeval che conterrà il 
tempo di sistema.
Il secondo argomento è un puntatore a una struttura timezone, ma in questo caso 
viene fornito NULL poiché non è necessario specificare un fuso orario.
La funzione gettimeofday restituisce 0 in caso di successo e -1 
in caso di errore. 
Nel codice fornito, se gettimeofday restituisce un valore diverso da 0, 
viene chiamata la funzione error per gestire l'errore e 
viene restituito un valore di errore.

Il tempo ottenuto da gettimeofday viene quindi convertito in millisecondi 
usando la formula (tv.tv_sec * (u_int64_t)1000) + (tv.tv_usec / 1000). 
La struttura timeval ha due campi: tv_sec, che rappresenta i secondi, e tv_usec, 
che rappresenta i microsecondi. La formula combina i secondi convertiti 
in millisecondi e i microsecondi divisi per 1000 per ottenere il tempo totale 
in millisecondi.

In sintesi, la funzione get_time restituisce il tempo attuale in millisecondi, 
utilizzando la funzione di sistema gettimeofday, per calcolare il 
tempo trascorso. Questa funzione può essere utilizzata per misurare 
intervalli di tempo o per ottenere timestamp per scopi di 
registrazione o monitoraggio. */


/* La funzione ft_usleep è una funzione personalizzata che introduce
una pausa (delay) nell'esecuzione del programma 
per un determinato numero di microsecondi. */

// Controllo subito con questa funzione che i caratteri inseriti siano giusti

-----------------------------------------------------------------------------------------

PHILOSOPHER.H

STRUCT PHILO

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

	 STRUCT DATA

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


