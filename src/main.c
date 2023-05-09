/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfalasch <pfalasch@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 19:35:41 by pfalasch          #+#    #+#             */
/*   Updated: 2023/05/09 17:39:44 by pfalasch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosopher.h"


/* in questa funzione vogliamo allocare la memoria necessaria per i filosofi
	le forchette e che cosáltro?
 */
int	ft_alloc_mem(t_data *data, int ac. char **av)
{
	
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
	data->philo_num = (int)ft_atoi(av[1]);
	data->death_time = (u_int64_t)ft_atoi(av[2]);
	data->eat_time = (u_int64_t)ft_atoi(av[3]);
	data->sleep_time = (u_int64_t)ft_atoi(av[4]);
	if (ac == 6)
		data->meals_nb = (int)ft_atoi(av[5]);
	else
		data->meals_nb = -1;
	if (data->philo_num <= 0 || data->philo_num >= 200 || data->death_time < 0
		|| data->eat_time < 0 || data->sleep_time < 0)
	{
		printf("uno degli argomento ha un valore errato\n");
		printf("prendi spunto dal seguente: 5 800 200 200 7\n");
		return (1);
	}
	data->dead = 0;
	data->stop = 0;
	pthread_mutex_init(&data->write, NULL);
	pthread_mutex_init(&data->lock, NULL);
}
/* 	questa funzione mi serve per inizializzare tutti i valori delle struct e per
	eseguire i check del caso per gli input */

	int ft_init(t_data *data, int **ac, int av)
{
	if (ft_init_data(data, av, ac))
		return (1);
	if ()
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
	if (ft_init(&data, av, ac))
		return (1);
}