/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: machrist <machrist@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 20:01:12 by machrist          #+#    #+#             */
/*   Updated: 2024/08/10 01:04:54 by machrist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	check_is_dead(t_philosopher *philosopher)
{
	unsigned long long	time;

	pthread_mutex_lock(philosopher->is_dead_mutex);
	if (*philosopher->is_dead)
		return (pthread_mutex_unlock(philosopher->is_dead_mutex), true);
	pthread_mutex_unlock(philosopher->is_dead_mutex);
	time = get_time_ms();
	if (time - philosopher->last_meal > philosopher->time_to_die)
		return (print_msg(philosopher, MSG_DIE), true);
	return (false);
}

void	start_eating(t_philosopher *philosopher)
{
	print_msg(philosopher, MSG_EAT);
	philosopher->last_meal = get_time_ms();
	if (ft_usleep(philosopher, EAT))
		return ;
	pthread_mutex_lock(&philosopher->is_eating_mutex);
	if (philosopher->nb_eat > 0)
		philosopher->nb_eat--;
	pthread_mutex_unlock(&philosopher->is_eating_mutex);
	philosopher_lock_forks(philosopher);
	philosopher->forks_left = true;
	*philosopher->forks_right = true;
	philosopher_unlock_forks(philosopher);
	print_msg(philosopher, MSG_SLEEP);
	if (ft_usleep(philosopher, SLEEP))
		return ;
	print_msg(philosopher, MSG_THINK);
	if (ft_usleep(philosopher, THINK))
		return ;
}

void	take_fork(t_philosopher *philosopher)
{
	philosopher_lock_forks(philosopher);
	if (philosopher->forks_left && *philosopher->forks_right)
	{
		philosopher->forks_left = false;
		*philosopher->forks_right = false;
		print_msg(philosopher, MSG_FORK);
		print_msg(philosopher, MSG_FORK);
		philosopher_unlock_forks(philosopher);
		start_eating(philosopher);
	}
	else
		philosopher_unlock_forks(philosopher);
}

void	*routine(void *arg)
{
	t_philosopher	*philosopher;

	philosopher = (t_philosopher *)arg;
	if (*philosopher->nb_philo == 1)
	{
		print_msg(philosopher, MSG_FORK);
		usleep(philosopher->time_to_die * 1000);
		print_msg(philosopher, MSG_DIE);
		return (NULL);
	}
	if (philosopher->id % 2)
		usleep(10000);
	while (!check_is_dead(philosopher))
		take_fork(philosopher);
	return (NULL);
}
