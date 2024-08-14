/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_msg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: machrist <machrist@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:35:20 by machrist          #+#    #+#             */
/*   Updated: 2024/08/14 18:07:48 by machrist         ###   ########.fr       */
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
	pthread_mutex_lock(philosopher->is_dead_mutex);
	if (*philosopher->is_dead)
	{
		pthread_mutex_unlock(philosopher->is_dead_mutex);
		return ;
	}
	if (!ft_strncmp(msg, MSG_FORK, 4))
	{
		printf("%lld %d %s", get_time_ms() - *philosopher->start,
			philosopher->id + 1, msg);
		printf("%lld %d %s", get_time_ms() - *philosopher->start,
			philosopher->id + 1, msg);
	}
	else
		printf("%lld %d %s", get_time_ms() - *philosopher->start,
			philosopher->id + 1, msg);
	pthread_mutex_unlock(philosopher->is_dead_mutex);
}
