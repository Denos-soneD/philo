/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: machrist <machrist@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 01:25:36 by machrist          #+#    #+#             */
/*   Updated: 2024/08/14 21:12:01 by machrist         ###   ########.fr       */
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
	unsigned long long	time_now;

	time_now = get_time_ms();
	if (act == EAT)
		time = time_now + philosopher->time_to_eat;
	else
		time = time_now + philosopher->time_to_sleep;
	while (time_now < time)
	{
		if (time_now - philosopher->last_meal > philosopher->time_to_die)
			return (print_msg(philosopher, MSG_DIE), true);
		if (check_is_dead(philosopher))
			return (true);
		usleep(5000);
		time_now = get_time_ms();
	}
	return (false);
}
