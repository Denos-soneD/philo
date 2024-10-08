/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: machrist <machrist@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:17:36 by machrist          #+#    #+#             */
/*   Updated: 2024/08/10 17:03:02 by machrist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_philo	philo;

	if (ac != 5 && ac != 6)
	{
		printf("Error: wrong number of arguments\n");
		return (1);
	}
	if (init_philo(&philo, ac, av))
		return (1);
	if (init_threads(&philo))
		return (1);
	pthread_mutex_destroy(&philo.is_dead_mutex);
	pthread_mutex_destroy(&philo.monitor_mutex);
	destroy_forks_mutex(&philo);
	free(philo.philosopher);
	return (0);
}
