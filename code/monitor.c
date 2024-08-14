/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: machrist <machrist@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 18:57:39 by machrist          #+#    #+#             */
/*   Updated: 2024/08/14 17:55:23 by machrist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	check_finish_eat(t_philo *philo, int i)
{
	philosopher_lock_forks(&philo->philosopher[i]);
	if (philo->philosopher[i].nb_eat == 0 && !philo->philosopher[i].finish_eat)
	{
		philo->philosopher[i].finish_eat = true;
		philo->nb_philo_eat++;
		if (philo->nb_philo == philo->nb_philo_eat)
		{
			philosopher_unlock_forks(&philo->philosopher[i]);
			pthread_mutex_lock(&philo->is_dead_mutex);
			philo->is_dead = true;
			pthread_mutex_unlock(&philo->is_dead_mutex);
			return (true);
		}
	}
	philosopher_unlock_forks(&philo->philosopher[i]);
	return (false);
}

bool	ft_check(t_philo *philo, int i)
{
	pthread_mutex_lock(&philo->monitor_mutex);
	if (philo->monitor_stop)
	{
		pthread_mutex_unlock(&philo->monitor_mutex);
		return (true);
	}
	pthread_mutex_unlock(&philo->monitor_mutex);
	if (check_finish_eat(philo, i))
		return (true);
	return (false);
}

void	*monitor(void *arg)
{
	t_philo	*philo;
	int		i;

	philo = (t_philo *)arg;
	if (philo->nb_philo == 1)
		return (NULL);
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
