/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks_mutex.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: machrist <machrist@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:37:33 by machrist          #+#    #+#             */
/*   Updated: 2024/09/02 16:18:03 by machrist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	init_forks_mutex(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->nb_philo)
	{
		if (pthread_mutex_init(&philo->philosopher[i].forks_mutex_left, NULL))
			return (clear_fork_mutex(philo, i));
		if (i == philo->nb_philo - 1)
		{
			philo->philosopher[i].forks_mutex_right = \
				&philo->philosopher[0].forks_mutex_left;
			philo->philosopher[i].forks_right = \
				&philo->philosopher[0].forks_left;
		}
		else
		{
			philo->philosopher[i].forks_mutex_right = &philo->philosopher[i
				+ 1].forks_mutex_left;
			philo->philosopher[i].forks_right = &philo->philosopher[i
				+ 1].forks_left;
		}
		i++;
	}
	return (false);
}

void	destroy_forks_mutex(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->nb_philo)
	{
		pthread_mutex_destroy(&philo->philosopher[i].forks_mutex_left);
		i++;
	}
}

void	philosopher_unlock_forks(t_philosopher *philosopher)
{
	if (philosopher->id % 2 == 0)
	{
		pthread_mutex_unlock(&philosopher->forks_mutex_left);
		pthread_mutex_unlock(philosopher->forks_mutex_right);
	}
	else
	{
		pthread_mutex_unlock(philosopher->forks_mutex_right);
		pthread_mutex_unlock(&philosopher->forks_mutex_left);
	}
}

void	philosopher_lock_forks(t_philosopher *philosopher)
{
	if (philosopher->id % 2 == 0)
	{
		pthread_mutex_lock(&philosopher->forks_mutex_left);
		pthread_mutex_lock(philosopher->forks_mutex_right);
	}
	else
	{
		pthread_mutex_lock(philosopher->forks_mutex_right);
		pthread_mutex_lock(&philosopher->forks_mutex_left);
	}
}
