/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfalasch <pfalasch@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 19:38:10 by pfalasch          #+#    #+#             */
/*   Updated: 2023/05/16 15:27:21 by pfalasch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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