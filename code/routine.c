/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: machrist <machrist@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 20:01:12 by machrist          #+#    #+#             */
/*   Updated: 2024/02/29 18:28:39 by machrist         ###   ########.fr       */
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

void	check_forks(t_philosopher *philosopher)
{
	if (philosopher->fork[philosopher->id]
		&& philosopher->fork[philosopher->id_right])
	{
		philosopher->fork[philosopher->id] -= 1;
		philosopher->fork[philosopher->id_right] -= 1;
		print_msg(philosopher, MSG_FORK);
	}
	else if (philosopher->fork[philosopher->id_right]
		&& philosopher->fork[philosopher->id_left])
	{
		philosopher->fork[philosopher->id_right] -= 1;
		philosopher->fork[philosopher->id_left] -= 1;
		print_msg(philosopher, MSG_FORK);
		print_msg(philosopher, MSG_FORK);
	}
	else
		printf("Error: philosopher can't take a fork\n");
}

void	start_sleeping(t_philosopher *philosopher)
{
	print_msg(philosopher, MSG_SLEEP);
	if (check_is_dead(philosopher, SLEEP))
		return ;
	usleep(philosopher->time_to_sleep * 1000);
	print_msg(philosopher, MSG_THINK);
}

void	start_eating(t_philosopher *philosopher)
{
	check_forks(philosopher);
	pthread_mutex_unlock(philosopher->forks_mutex);
	print_msg(philosopher, MSG_EAT);
	philosopher->last_meal = get_time_ms();
	usleep(philosopher->time_to_eat * 1000);
	pthread_mutex_lock(philosopher->forks_mutex);
	philosopher->fork[philosopher->id] = 2;
	pthread_mutex_unlock(philosopher->forks_mutex);
	philosopher->nb_eat--;
	start_sleeping(philosopher);
}

void	*routine(void *arg)
{
	t_philosopher	*philosopher;

	philosopher = (t_philosopher *)arg;
	if (philosopher->id % 2)
		usleep(60000);
	while (1)
	{
		pthread_mutex_lock(philosopher->forks_mutex);
		if ((philosopher->fork[philosopher->id]
				&& philosopher->fork[philosopher->id_right])
			|| (philosopher->fork[philosopher->id_right]
				&& philosopher->fork[philosopher->id_left]))
			start_eating(philosopher);
		else
			pthread_mutex_unlock(philosopher->forks_mutex);
		if (check_is_dead(philosopher, THINK))
			return (NULL);
	}
	return (NULL);
}
