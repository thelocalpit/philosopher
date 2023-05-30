/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tasks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfalasch <pfalasch@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 12:11:33 by pfalasch          #+#    #+#             */
/*   Updated: 2023/05/30 17:30:46 by pfalasch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosopher.h"

void	*monitor(void *philo_p)
{
	t_philo	*philo;

	philo = (t_philo *)philo_p;
	while (philo->data->dead == 0)
	{
		pthread_mutex_lock(&philo->lock);
		if (philo->data->stop >= philo->data->philo_nb)
			philo->data->dead = 1;
		pthread_mutex_unlock(&philo->lock);
	}
	return ((void *)0);
}

void	*supervisor(void *philo_p)
{
	t_philo	*philo;

	philo = (t_philo *)philo_p;
	while (philo->data->dead == 0)
	{
		pthread_mutex_lock(&philo->lock);
		if (get_time() >= philo->time_to_die && philo->eating == 0)
			messages("died", philo);
		if (philo->eat_count == philo->data->meals_nb)
		{
			pthread_mutex_lock(&philo->data->lock);
			philo->data->stop++;
			philo->eat_count++;
			pthread_mutex_unlock(&philo->data->lock);
		}
		pthread_mutex_unlock(&philo->lock);
	}
	return ((void *)0);
}

void	*routine(void *philo_p)
{
	t_philo	*philo;

	philo = (t_philo *)philo_p;
	philo->time_to_die = get_time() + philo->data->death_time;
	if (pthread_create(&philo->t_super, NULL, &supervisor, (void *)philo))
		return ((void *)1);
	while (philo->data->dead == 0)
	{
		eat(philo);
		messages("is thinking", philo);
	}
	if (pthread_join(philo->t_super, NULL))
		return ((void *)1);
	return ((void *)0);
}
