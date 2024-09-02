/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: machrist <machrist@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 20:01:12 by machrist          #+#    #+#             */
/*   Updated: 2024/09/02 15:30:16 by machrist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	start_eating(t_philosopher *philosopher)
{
	print_msg(philosopher, MSG_EAT);
	philosopher->last_meal = get_time_ms();
	if (ft_usleep(philosopher, EAT))
		return ;
	philosopher_lock_forks(philosopher);
	if (philosopher->nb_eat > 0)
		philosopher->nb_eat--;
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
		philosopher_unlock_forks(philosopher);
		print_msg(philosopher, MSG_FORK);
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
		printf("%lld %d %s", get_time_ms() - *philosopher->start,
			philosopher->id + 1, MSG_FORK);
		usleep(philosopher->time_to_die * 1000);
		print_msg(philosopher, MSG_DIE);
		return (NULL);
	}
	if (!(philosopher->id % 2))
		usleep(60000);
	while (!check_is_dead(philosopher, get_time_ms()))
		take_fork(philosopher);
	return (NULL);
}
