/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: machrist <machrist@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 18:57:39 by machrist          #+#    #+#             */
/*   Updated: 2024/08/10 01:06:30 by machrist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	check_finish_eat(t_philo *philo, int i)
{
	pthread_mutex_lock(&philo->philosopher[i].is_eating_mutex);
	if (philo->philosopher[i].nb_eat == 0 && !philo->philosopher[i].finish_eat)
	{
		philo->philosopher[i].finish_eat = true;
		philo->nb_philo_eat++;
		if (philo->nb_philo == philo->nb_philo_eat)
		{
			pthread_mutex_unlock(&philo->philosopher[i].is_eating_mutex);
			pthread_mutex_lock(&philo->is_dead_mutex);
			philo->is_dead = true;
			pthread_mutex_unlock(&philo->is_dead_mutex);
			return (true);
		}
	}
	pthread_mutex_unlock(&philo->philosopher[i].is_eating_mutex);
	return (false);
}

bool	ft_check(t_philo *philo, int i)
{
	pthread_mutex_lock(&philo->is_dead_mutex);
	if (philo->is_dead)
	{
		pthread_mutex_unlock(&philo->is_dead_mutex);
		return (true);
	}
	pthread_mutex_unlock(&philo->is_dead_mutex);
	if (check_finish_eat(philo, i))
		return (true);
	return (false);
}

void	*monitor(void *arg)
{
	t_philo	*philo;
	int		i;

	philo = (t_philo *)arg;
	while (1)
	{
		i = 0;
		while (i < philo->nb_philo)
		{
			if (ft_check(philo, i))
				return (NULL);
			i++;
		}
	}
	return (NULL);
}
