/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: machrist <machrist@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:15:16 by machrist          #+#    #+#             */
/*   Updated: 2024/02/20 18:42:26 by machrist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	free_philo(t_philo *philo, char *msg)
{
	printf("%s", msg);
	if (philo->philosopher)
		free(philo->philosopher);
	pthread_mutex_destroy(&philo->mutex);
	return (1);
}

bool	init_threads(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->nb_philo)
	{
		if (pthread_create(&philo->philosopher[i].th, NULL, &routine,
				philo->philosopher + i))
		{
			free_philo(philo, MSG_PTHREAD_CRT);
			return (true);
		}
		i++;
	}
	i = 0;
	while (i < philo->nb_philo)
	{
		if (pthread_join(philo->philosopher[i].th, NULL))
		{
			free_philo(philo, MSG_PTHREAD_JOIN);
			return (true);
		}
		i++;
	}
	return (false);
}

void	init_philosopher(t_philo *philo, int ac, char **av)
{
	int	i;

	i = 0;
	while (i < philo->nb_philo)
	{
		philo->philosopher[i].id = i + 1;
		philo->philosopher[i].nb_fork = &philo->nb_fork;
		philo->philosopher[i].time_to_die = ft_atoi(av[2]);
		philo->philosopher[i].time_to_eat = ft_atoi(av[3]);
		philo->philosopher[i].time_to_sleep = ft_atoi(av[4]);
		philo->philosopher[i].last_meal = philo->start;
		philo->philosopher[i].start = &philo->start;
		philo->philosopher[i].is_dead = &philo->is_dead;
		philo->philosopher[i].mutex = &philo->mutex;
		if (ac == 5)
			philo->philosopher[i].nb_eat = -1;
		if (ac == 6)
			philo->philosopher[i].nb_eat = ft_atoi(av[5]);
		i++;
	}
}

bool	init_philo(t_philo *philo, int ac, char **av)
{
	if (ft_atoi(av[1]) < 1 || ft_atoi(av[2]) < 0
		|| ft_atoi(av[3]) < 0 || ft_atoi(av[4]) < 0
		|| (ac == 6 && ft_atoi(av[5]) < 1))
	{
		printf("Error: wrong arguments\n");
		return (true);
	}
	pthread_mutex_init(&philo->mutex, NULL);
	philo->nb_philo = ft_atoi(av[1]);
	philo->nb_fork = philo->nb_philo;
	philo->start = get_time_ms();
	philo->is_dead = false;
	philo->philosopher = malloc(sizeof(t_philosopher) * philo->nb_philo);
	if (!philo->philosopher)
	{
		free_philo(philo, MSG_MALLOC);
		return (true);
	}
	init_philosopher(philo, ac, av);
	return (false);
}
