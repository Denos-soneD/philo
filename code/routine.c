/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: machrist <machrist@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 20:01:12 by machrist          #+#    #+#             */
/*   Updated: 2024/02/21 16:09:42 by machrist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	check_is_dead(t_philosopher *philosopher, int act)
{
	unsigned long long	time;

	pthread_mutex_lock(philosopher->mutex);
	if (philosopher->nb_eat == 0 || *philosopher->is_dead)
		return (pthread_mutex_unlock(philosopher->mutex), true);
	pthread_mutex_unlock(philosopher->mutex);
	time = get_time_ms();
	time = time - philosopher->last_meal;
	if (time > (unsigned long long)philosopher->time_to_die)
		return (print_msg(philosopher, MSG_DIE), true);
	if (act == SLEEP)
	{
		time = time + philosopher->time_to_sleep;
		if (time > (unsigned long long)philosopher->time_to_die)
		{
			while (get_time_ms()
				- philosopher->last_meal < philosopher->time_to_die)
				usleep(1000);
			print_msg(philosopher, MSG_DIE);
			return (true);
		}
	}
	return (false);
}

bool	start_eating(t_philosopher *philosopher)
{
	print_msg(philosopher, MSG_EAT);
	philosopher->last_meal = get_time_ms();
	usleep(philosopher->time_to_eat * 1000);
	pthread_mutex_lock(philosopher->mutex);
	*philosopher->nb_fork += 2;
	pthread_mutex_unlock(philosopher->mutex);
	philosopher->nb_eat--;
	return (false);
}

bool	start_sleeping(t_philosopher *philosopher)
{
	print_msg(philosopher, MSG_SLEEP);
	if (check_is_dead(philosopher, SLEEP))
		return (true);
	usleep(philosopher->time_to_sleep * 1000);
	print_msg(philosopher, MSG_THINK);
	usleep(1000);
	return (false);
}

void	*routine(void *arg)
{
	t_philosopher	*philosopher;

	philosopher = (t_philosopher *)arg;
	while (1)
	{
		pthread_mutex_lock(philosopher->mutex);
		if (*philosopher->nb_fork > 1)
		{
			*philosopher->nb_fork -= 2;
			pthread_mutex_unlock(philosopher->mutex);
			print_msg(philosopher, MSG_FORK);
			if (start_eating(philosopher) || start_sleeping(philosopher))
				return (NULL);
		}
		else
			pthread_mutex_unlock(philosopher->mutex);
		if (check_is_dead(philosopher, THINK))
			return (NULL);
	}
	return (NULL);
}
