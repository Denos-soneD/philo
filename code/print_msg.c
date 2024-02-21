/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_msg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: machrist <machrist@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:35:20 by machrist          #+#    #+#             */
/*   Updated: 2024/02/21 16:12:21 by machrist         ###   ########.fr       */
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

void	print_msg(t_philosopher *philosopher, char *msg)
{
	unsigned long long	time;

	pthread_mutex_lock(philosopher->mutex);
	if (*philosopher->is_dead || philosopher->nb_eat == 0)
	{
		pthread_mutex_unlock(philosopher->mutex);
		return ;
	}
	pthread_mutex_unlock(philosopher->mutex);
	time = get_time_ms();
	time = time - *philosopher->start;
	printf("%lld %d %s", time, philosopher->id, msg);
	if (ft_strncmp(msg, MSG_DIE, 4) == 0)
	{
		pthread_mutex_lock(philosopher->mutex);
		*philosopher->is_dead = true;
		pthread_mutex_unlock(philosopher->mutex);
	}
}
