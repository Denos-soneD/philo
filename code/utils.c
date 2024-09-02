/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: machrist <machrist@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 01:25:36 by machrist          #+#    #+#             */
/*   Updated: 2024/09/02 15:36:24 by machrist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	validate_arguments(int ac, char **av)
{
	if (ft_atoi(av[1]) < 1 || ft_atoi(av[2]) < 0 || ft_atoi(av[3]) < 0
		|| ft_atoi(av[4]) < 0 || (ac == 6 && ft_atoi(av[5]) < 1))
	{
		printf("Error: wrong arguments\n");
		return (false);
	}
	return (true);
}

bool	check_is_dead(t_philosopher *philosopher, long long time_now)
{
	pthread_mutex_lock(philosopher->is_dead_mutex);
	if (*philosopher->is_dead || time_now == -1)
	{
		pthread_mutex_unlock(philosopher->is_dead_mutex);
		return (true);
	}
	pthread_mutex_unlock(philosopher->is_dead_mutex);
	if (time_now - philosopher->last_meal > philosopher->time_to_die)
		return (print_msg(philosopher, MSG_DIE), true);
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
	long long	time;
	long long	time_now;

	time_now = get_time_ms();
	if (act == EAT)
		time = time_now + philosopher->time_to_eat;
	else if (act == SLEEP)
		time = time_now + philosopher->time_to_sleep;
	else
		time = time_now + (philosopher->time_to_die - (philosopher->time_to_eat
					+ philosopher->time_to_sleep)) / 2;
	while (time_now < time)
	{
		usleep(1000);
		time_now = get_time_ms();
		if (check_is_dead(philosopher, time_now))
			return (true);
	}
	return (false);
}
