/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: machrist <machrist@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 20:01:12 by machrist          #+#    #+#             */
/*   Updated: 2024/06/06 19:49:50 by machrist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	check_is_dead(t_philosopher *philosopher, int act)
{
	unsigned long long	time;

	pthread_mutex_lock(philosopher->is_dead_mutex);
	if (philosopher->nb_eat == 0 || *philosopher->is_dead)
		return (pthread_mutex_unlock(philosopher->is_dead_mutex), true);
	pthread_mutex_unlock(philosopher->is_dead_mutex);
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

void	start_eating(t_philosopher *philosopher)
{
	unsigned long long	time;

	time = get_time_ms() + philosopher->time_to_eat;
	print_msg(philosopher, MSG_EAT);
	philosopher->last_meal = get_time_ms();
	while (get_time_ms() < time)
	{
		if (check_is_dead(philosopher, EAT))
			return ;
		usleep(1000);
	}
	philosopher->nb_eat--;
	pthread_mutex_lock(&philosopher->forks_mutex_left);
	pthread_mutex_lock(philosopher->forks_mutex_right);
	philosopher->forks_left = true;
	*philosopher->forks_right = true;
	philosopher_unlock_forks(philosopher);
	print_msg(philosopher, MSG_SLEEP);
	if (check_is_dead(philosopher, SLEEP))
		return ;
	usleep(philosopher->time_to_sleep * 1000);
	print_msg(philosopher, MSG_THINK);
	usleep(1000);
}

bool	check_philo(t_philosopher *philosopher)
{
	if (*philosopher->nb_philo == 1)
	{
		print_msg(philosopher, MSG_FORK);
		usleep(philosopher->time_to_die * 1000);
		pthread_mutex_unlock(&philosopher->forks_mutex_left);
		return (check_is_dead(philosopher, SLEEP));
	}
	return (false);
}

bool	take_fork(t_philosopher *philosopher)
{
	pthread_mutex_lock(&philosopher->forks_mutex_left);
	if (check_philo(philosopher))
		return (true);
	pthread_mutex_lock(philosopher->forks_mutex_right);
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
	return (false);
}

void	*routine(void *arg)
{
	t_philosopher	*philosopher;

	philosopher = (t_philosopher *)arg;
	if (philosopher->id % 2)
		usleep(60000);
	while (!check_is_dead(philosopher, THINK))
		if (take_fork(philosopher))
			return (NULL);
	return (NULL);
}
