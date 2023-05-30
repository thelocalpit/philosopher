/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfalasch <pfalasch@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 19:35:41 by pfalasch          #+#    #+#             */
/*   Updated: 2023/05/30 11:38:03 by pfalasch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosopher.h"

int	ft_one_philo(t_data *data)
{
	data->start_time = get_time();
	if (pthread_create(&data->tid[0], NULL, &routine, &data->philos[0]))
	{
		printf("errore mentre vengono creati i threads");
		return (ft_free_mem(data));
	}
	pthread_detach(data->tid[0]);
	while (data->dead == 0)
		ft_usleep(0);
	ft_free_mem(data);
	return (0);
}

int	ft_free_mem(t_data *data)
{
	int	i;

	if (data)
	{
		i = -1;
		while (++i < data->philo_nb)
		{
			pthread_mutex_destroy(&data->forks[i]);
			pthread_mutex_destroy(&data->philos[i].lock);
		}
		pthread_mutex_destroy(&data->write);
		pthread_mutex_destroy(&data->lock);
		if (data->tid)
			free(data->tid);
		if (data->forks)
			free(data->forks);
		if (data->philos)
			free(data->philos);
	}
	return (1);
}

int	main(int ac, char **av)
{
	t_data	data;

	if (ac < 5 || ac > 6)
	{
		printf("controlla gli input, qualcosa non torna!");
		return (1);
	}
	if (input_checker(av))
		return (1);
	if (ft_init(&data, ac, av))
		return (1);
	if (data.philo_nb == 1)
		return (ft_one_philo(&data));
	if (thread_init(&data))
		return (1);
	ft_free_mem(&data);
	return (0);
}
