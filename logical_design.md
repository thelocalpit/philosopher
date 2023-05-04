IN QUESTO FILE STRUTTURIAMO IL DESIGN DEL PROGETTO PHILOSOPHER. 

1. controllo degli input

	in questa prima fase controlliamo il corretto inserimento degli input.
	uno standard input è 5 800 200 200 7 (numero di philo, tempo in cui un philo muore, tempo in cui un philo mangia, tempo in cui un philo dorme, numero di mangiate che ciascun philo deve fare per terminare il programma)

	max 200 philos. min 1
	il resto degli input deve essere maggiore di 0.
	tutti i valori sono in digits.

	le strutture da creare sono una necessaria per definire tutte le caratteristiche di un philo.

		- 	dovrò dire quale è lo status attuale del philo
		- 	quante volte ha mangiato
		-	se sta ancora mangiando (perchè potrebbe succedere che risulti aver 	comincaito a mangiare già il numero richiesto di volte ma che non abbia finito, quindi dobbiamo tenere di conto se il thread è ancora in atto)
		- 	id. che serve ad identificare che philo stiamo trattando.
		- dovrò creare delle variabili che mi indichino se le forchette sono disponibili o occupate. e nel caso siano disponibili entrambe utilizzarle e metterle in blocco per gli altri.

	le altre per gestire i dati generali da tenere sotto controllo. 
		- il numero di philo totali
		- quanti hanno finito
		- quanti sono morti
		- dati di input.

	una volta create le strutture dobbiamo allocare e inizializzare tutte le variabili. 
	la funzione da usare è pthread_mutex_t mutex;
	dobbiamo avviare il processo attivando i threads. (wtf come si fa??!!)
	-	necessario quindi scrivere il main e impostare le funzioni per avviare i threads


	una volta avviati tutti i processi iniziali strutturemo il programma dividendo in 3 main functions:
	-	routine. ovvero ci serve per far fare tutte le azioni al philo. quindi posso mangiare? no allora penso, no allora dormo. 
	-	un supervisore. che serve appunto per controllare lo stato di ciascun philo. sta per morire? può mangiare? ecc..
	-	un monitor che controlla quanto tutti hanno mangiato e possiamo terminare il programma. 


	il routine è un loop che viene avviato continuamente e aggiorna continuamente lo stato dei philos. è anche la funzione dove viene avviata anche la funzione di supervisor per controllare che threads avviati siano i più ottimali e che non si creino stalli strani.
	devono mangiare, dormire e pensare (o morire merde). 
	un philo deve pensare quando non mangia. e fin qui tutto bene. "philo is thinking".
	se mangia: picking the forks, eating, put down the fork, sleep. 
	per prendere le forchette facciamo un lock con il mutex. prima blocchi la forchetta di dx se facciamo in senso orario. 
	prese le forchette mangerà sto cristiano, e lo diciamo nello status. in questo modo tutti sapranno che sta mangiando, compreso il supervisor che non lo farà morire. (come devo usare il usleep???)
	poi lascia le forks (utilizzando la stessa funzione usata per fargliela prendere ma sbloccando le forchette invece che lockarle). di nuovo il philo va a dormire. 


	il supervisor deve appunto controllare il tempo dall'ultima volta che il philo ha mangiato. insomma deve assicurarsi che non muoia di fame. e lasciarlo vivere se sta mangiando. 
	controlliamo anche quante volte ha mangiato e quante altre volte deve mangiare. 

	il monitor serve solo a controllare se tutti hanno finito. in quel caso terminiamo tutti i threads.

	puliamo la memoria


	nb. da capire bene come utilizzare mutex. come inizializzare le cose. 


	nb. prima di dare in disponibilità le forchette a un determinato philo, dobbiamo assicurarci che nessuno stia per morire, perchè altrimenti quel philo ha la priorità. 
