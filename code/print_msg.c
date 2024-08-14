/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_msg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: machrist <machrist@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:35:20 by machrist          #+#    #+#             */
/*   Updated: 2024/08/14 21:14:26 by machrist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned long long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((unsigned long long)(tv.tv_sec * 1000 + tv.tv_usec / 1000));
}

void	print_msg(t_philosopher *philosopher, char *msg)
{
	unsigned long long	time;

	pthread_mutex_lock(philosopher->is_dead_mutex);
	if (*philosopher->is_dead)
	{
		pthread_mutex_unlock(philosopher->is_dead_mutex);
		return ;
	}
	time = get_time_ms();
	if (!ft_strncmp(msg, MSG_FORK, 4))
	{
		printf("%lld %d %s", time - *philosopher->start, philosopher->id + 1,
			msg);
		printf("%lld %d %s", time - *philosopher->start, philosopher->id + 1,
			msg);
	}
	else
		printf("%lld %d %s", time - *philosopher->start, philosopher->id + 1,
			msg);
	if (!ft_strncmp(msg, MSG_DIE, 4))
		*philosopher->is_dead = true;
	pthread_mutex_unlock(philosopher->is_dead_mutex);
}
