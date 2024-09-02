/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: machrist <machrist@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 15:53:08 by machrist          #+#    #+#             */
/*   Updated: 2024/09/02 16:10:25 by machrist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	clear_init(t_philo *philo)
{
	if (!philo->philosopher || philo->start == -1)
	{
		if (philo->philosopher)
			free(philo->philosopher);
		else
			printf(MSG_MALLOC);
		pthread_mutex_destroy(&philo->is_dead_mutex);
		pthread_mutex_destroy(&philo->monitor_mutex);
		return (true);
	}
	return (false);
}

bool	clear_fork_mutex(t_philo *philo, int i)
{
	printf(MSG_PTHREAD_MUTEX);
	while (i--)
		pthread_mutex_destroy(&philo->philosopher[i].forks_mutex_left);
	free(philo->philosopher);
	pthread_mutex_destroy(&philo->is_dead_mutex);
	pthread_mutex_destroy(&philo->monitor_mutex);
	return (true);
}
