/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: machrist <machrist@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 01:25:36 by machrist          #+#    #+#             */
/*   Updated: 2024/08/14 18:08:04 by machrist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	check_is_dead(t_philosopher *philosopher)
{
	pthread_mutex_lock(philosopher->is_dead_mutex);
	if (*philosopher->is_dead)
	{
		pthread_mutex_unlock(philosopher->is_dead_mutex);
		return (true);
	}
	if (get_time_ms() - philosopher->last_meal > philosopher->time_to_die)
	{
		printf("%lld %d %s", get_time_ms() - *philosopher->start,
			philosopher->id + 1, MSG_DIE);
		*philosopher->is_dead = true;
		pthread_mutex_unlock(philosopher->is_dead_mutex);
		return (true);
	}
	pthread_mutex_unlock(philosopher->is_dead_mutex);
	return (false);
}

void	wait_all_threads(t_philo *philo, int i)
{
	if (i == -1)
		return ;
	while (i--)
		pthread_join(philo->philosopher[i].th, NULL);
	pthread_mutex_lock(&philo->monitor_mutex);
	philo->monitor_stop = true;
	pthread_mutex_unlock(&philo->monitor_mutex);
	pthread_join(philo->th, NULL);
}

bool	ft_usleep(t_philosopher *philosopher, int act)
{
	unsigned long long	time;

	if (act == EAT)
		time = get_time_ms() + philosopher->time_to_eat;
	else if (act == SLEEP)
		time = get_time_ms() + philosopher->time_to_sleep;
	else
	{
		time = get_time_ms() + (philosopher->time_to_die
				- (philosopher->time_to_eat + philosopher->time_to_sleep)) / 2;
	}
	while (get_time_ms() < time)
	{
		if (check_is_dead(philosopher))
			return (true);
		usleep(1000);
	}
	return (false);
}
