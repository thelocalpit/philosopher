/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfalasch <pfalasch@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 19:38:06 by pfalasch          #+#    #+#             */
/*   Updated: 2023/05/30 12:21:31 by pfalasch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../inc/philosopher.h"

int ft_thread_routine(t_data *data)
{
	int i;

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
	return (0);
}

int	ft_thread_mon(t_data *data)
{
	pthread_t t_mon;
	
	if (data->meals_nb > 0)
	{
		if (pthread_create(&t_mon, NULL, &monitor, &data->philos[0]))
		{
			printf("errore mentre vengono creati i threads");
			ft_free_mem(data);
			return (1);
		}
	}
	return (0);
}

int	ft_thread_join(t_data *data)
{
	int i;

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

int	thread_init(t_data *data)
{
	data->start_time = get_time();
	if (ft_thread_mon(data))
		return (1);
	if (ft_thread_routine(data))
		return (1);
	if (ft_thread_join(data))
		return (1);
	return (0);
}
