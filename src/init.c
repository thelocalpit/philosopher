/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfalasch <pfalasch@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 19:38:00 by pfalasch          #+#    #+#             */
/*   Updated: 2023/05/30 17:22:50 by pfalasch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosopher.h"

int	ft_alloc_mem(t_data *data)
{
	data->tid = malloc(sizeof(pthread_t) * data->philo_nb);
	if (!data->tid)
	{
		printf("errore nell'allocazione dei threadssss");
		return (ft_free_mem(data));
	}
	data->forks = malloc(sizeof(pthread_mutex_t) * data->philo_nb);
	if (!data->forks)
	{
		printf("errore nell'allocazione delle forchette diobbbono");
		return (ft_free_mem(data));
	}
	data->philos = malloc(sizeof(t_philo) * data->philo_nb);
	if (!data->philos)
	{
		printf("errore nell'allocazione dei filosofi per Diana!");
		return (ft_free_mem(data));
	}
	return (0);
}

int	ft_init_forks(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->philo_nb)
		pthread_mutex_init(&data->forks[i], NULL);
	data->philos[0].l_fork = &data->forks[data->philo_nb - 1];
	data->philos[0].r_fork = &data->forks[0];
	i = 1;
	while (i < data->philo_nb)
	{
		data->philos[i].l_fork = &data->forks[i];
		data->philos[i].r_fork = &data->forks[i - 1];
		i++;
	}
	return (0);
}

void	ft_init_philo(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->philo_nb)
	{
		data->philos[i].data = data;
		data->philos[i].id = i + 1;
		data->philos[i].eat_count = 0;
		data->philos[i].eating = 0;
		data->philos[i].time_to_die = data->death_time;
		pthread_mutex_init(&data->philos[i].lock, NULL);
	}
}

int	ft_init_data(t_data *data, int ac, char **av)
{
	data->philo_nb = (int)ft_atoi(av[1]);
	data->death_time = (u_int64_t)ft_atoi(av[2]);
	data->eat_time = (u_int64_t)ft_atoi(av[3]);
	data->sleep_time = (u_int64_t)ft_atoi(av[4]);
	if (ac == 6)
		data->meals_nb = (int)ft_atoi(av[5]);
	else
		data->meals_nb = -1;
	if (data->philo_nb <= 0 || data->philo_nb > 200
		|| data->death_time < 0 || data->eat_time < 0 || data->sleep_time < 0)
	{
		printf("uno degli argomento ha un valore errato\n");
		printf("prendi spunto dal seguente: 5 800 200 200 7\n");
		return (1);
	}
	data->dead = 0;
	data->stop = 0;
	pthread_mutex_init(&data->write, NULL);
	pthread_mutex_init(&data->lock, NULL);
	return (0);
}

int	ft_init(t_data *data, int ac, char **av)
{
	if (ft_init_data(data, ac, av))
		return (1);
	if (ft_alloc_mem(data))
		return (1);
	if (ft_init_forks(data))
		return (1);
	ft_init_philo(data);
	return (0);
}
