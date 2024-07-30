/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: machrist <machrist@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 20:01:12 by machrist          #+#    #+#             */
/*   Updated: 2024/07/30 15:08:51 by machrist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	check_nb_eat(t_philosopher *philosopher)
{
	if (philosopher->nb_eat == 0)
	{
		(*philosopher->nb_philo_eat)++;
		philosopher->nb_eat = -1;
		if (*philosopher->nb_philo_eat == *philosopher->nb_philo)
			*philosopher->is_dead = true;
	}
}

bool	check_is_dead(t_philosopher *philosopher)
{
	unsigned long long	time;

	pthread_mutex_lock(philosopher->is_dead_mutex);
	check_nb_eat(philosopher);
	if (*philosopher->is_dead)
		return (pthread_mutex_unlock(philosopher->is_dead_mutex), true);
	pthread_mutex_unlock(philosopher->is_dead_mutex);
	time = get_time_ms();
	if (time - philosopher->last_meal > philosopher->time_to_die)
		return (print_msg(philosopher, MSG_DIE), true);
	return (false);
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
				- (philosopher->time_to_eat + philosopher->time_to_sleep)) - 5;
	}
	while (get_time_ms() < time)
	{
		if (check_is_dead(philosopher))
			return (print_msg(philosopher, MSG_DIE), true);
		usleep(1000);
	}
	return (false);
}

void	start_eating(t_philosopher *philosopher)
{
	print_msg(philosopher, MSG_EAT);
	philosopher->last_meal = get_time_ms();
	if (ft_usleep(philosopher, EAT))
		return ;
	philosopher->nb_eat--;
	pthread_mutex_lock(&philosopher->forks_mutex_left);
	pthread_mutex_lock(philosopher->forks_mutex_right);
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
