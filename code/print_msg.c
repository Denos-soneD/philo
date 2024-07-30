/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_msg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: machrist <machrist@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:35:20 by machrist          #+#    #+#             */
/*   Updated: 2024/07/30 15:23:48 by machrist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned long long	get_time_ms(void)
{
	struct timeval		tv;
	unsigned long long	time;

	gettimeofday(&tv, NULL);
	time = (unsigned long long)(tv.tv_sec);
	time *= 1000;
	time += (unsigned long long)(tv.tv_usec) / 1000;
	return (time);
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

void	print_msg(t_philosopher *philosopher, char *msg)
{
	unsigned long long	time;

	pthread_mutex_lock(philosopher->is_dead_mutex);
	if (*philosopher->is_dead || philosopher->nb_eat == 0)
	{
		pthread_mutex_unlock(philosopher->is_dead_mutex);
		return ;
	}
	time = get_time_ms();
	time = time - *philosopher->start;
	printf("%lld %d %s", time, philosopher->id + 1, msg);
	if (ft_strncmp(msg, MSG_DIE, 4) == 0)
	{
		*philosopher->is_dead = true;
	}
	pthread_mutex_unlock(philosopher->is_dead_mutex);
}
