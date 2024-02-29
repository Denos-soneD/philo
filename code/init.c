/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: machrist <machrist@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:15:16 by machrist          #+#    #+#             */
/*   Updated: 2024/02/29 18:24:30 by machrist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	free_philo(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->forks_mutex);
	pthread_mutex_lock(&philo->is_dead_mutex);
	printf("%s", msg);
	philo->is_dead = true;
	pthread_mutex_unlock(&philo->is_dead_mutex);
	if (philo->fork)
		free(philo->fork);
	pthread_mutex_unlock(&philo->forks_mutex);
	if (philo->philosopher)
		free(philo->philosopher);
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

void	set_philosopher(t_philo *philo, long long i)
{
	philo->philosopher[i].id = i;
	philo->philosopher[i].id_right = (i + 1) % philo->nb_philo;
	philo->philosopher[i].fork = philo->fork;
	philo->philosopher[i].forks_mutex = &philo->forks_mutex;
	philo->philosopher[i].last_meal = philo->start;
	philo->philosopher[i].start = &philo->start;
	philo->philosopher[i].is_dead = &philo->is_dead;
	philo->philosopher[i].is_dead_mutex = &philo->is_dead_mutex;
	if (philo->nb_philo > 1)
		philo->fork[i] = 1;
	else
		philo->fork[i] = 0;	
}

void	init_philosopher(t_philo *philo, int ac, char **av)
{
	long long	i;

	i = -1;
	while (++i < philo->nb_philo)
	{
		set_philosopher(philo, i);
		if (i == 0)
			philo->philosopher[i].id_left = philo->nb_philo - 1;
		else
			philo->philosopher[i].id_left = i - 1;
		philo->philosopher[i].time_to_die = ft_atoi(av[2]);
		philo->philosopher[i].time_to_eat = ft_atoi(av[3]);
		philo->philosopher[i].time_to_sleep = ft_atoi(av[4]);
		if (ac == 5)
			philo->philosopher[i].nb_eat = -1;
		if (ac == 6)
			philo->philosopher[i].nb_eat = ft_atoi(av[5]);
	}
}

bool	init_philo(t_philo *philo, int ac, char **av)
{
	if (ft_atoi(av[1]) < 1 || ft_atoi(av[2]) < 0 || ft_atoi(av[3]) < 0
		|| ft_atoi(av[4]) < 0 || (ac == 6 && ft_atoi(av[5]) < 1))
	{
		printf("Error: wrong arguments\n");
		return (true);
	}
	pthread_mutex_init(&philo->is_dead_mutex, NULL);
	pthread_mutex_init(&philo->forks_mutex, NULL);
	philo->nb_philo = ft_atoi(av[1]);
	philo->start = get_time_ms();
	philo->is_dead = false;
	philo->philosopher = malloc(sizeof(t_philosopher) * philo->nb_philo);
	philo->fork = malloc(sizeof(int) * philo->nb_philo);
	if (!philo->philosopher || !philo->fork)
	{
		free_philo(philo, MSG_MALLOC);
		return (true);
	}
	init_philosopher(philo, ac, av);
	return (false);
}
