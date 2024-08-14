/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: machrist <machrist@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:15:16 by machrist          #+#    #+#             */
/*   Updated: 2024/08/14 17:40:15 by machrist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	free_philo(t_philo *philo, char *msg, int i)
{
	pthread_mutex_lock(&philo->is_dead_mutex);
	printf("%s", msg);
	philo->is_dead = true;
	pthread_mutex_unlock(&philo->is_dead_mutex);
	wait_all_threads(philo, i);
	destroy_forks_mutex(philo);
	if (philo->philosopher)
		free(philo->philosopher);
	pthread_mutex_destroy(&philo->is_dead_mutex);
	pthread_mutex_destroy(&philo->monitor_mutex);
	return (1);
}

bool	init_threads(t_philo *philo)
{
	int	i;

	if (pthread_create(&philo->th, NULL, &monitor, philo))
		return (free_philo(philo, MSG_PTHREAD_CRT, -1));
	i = 0;
	while (i < philo->nb_philo)
	{
		if (pthread_create(&philo->philosopher[i].th, NULL, &routine,
				philo->philosopher + i))
			return (free_philo(philo, MSG_PTHREAD_CRT, i));
		i++;
	}
	wait_all_threads(philo, philo->nb_philo);
	return (false);
}

void	set_philosopher(t_philo *philo, long long i, int ac, char **av)
{
	philo->philosopher[i].id = i;
	philo->philosopher[i].nb_philo = &philo->nb_philo;
	philo->philosopher[i].last_meal = philo->start;
	philo->philosopher[i].start = &philo->start;
	philo->philosopher[i].is_dead = &philo->is_dead;
	philo->philosopher[i].is_dead_mutex = &philo->is_dead_mutex;
	philo->philosopher[i].time_to_die = ft_atoi(av[2]);
	philo->philosopher[i].time_to_eat = ft_atoi(av[3]);
	philo->philosopher[i].time_to_sleep = ft_atoi(av[4]);
	philo->philosopher[i].forks_left = true;
	if (ac == 5)
		philo->philosopher[i].nb_eat = -1;
	if (ac == 6)
		philo->philosopher[i].nb_eat = ft_atoi(av[5]);
	philo->philosopher[i].finish_eat = false;
}

void	init_philosopher(t_philo *philo, int ac, char **av)
{
	long long	i;

	i = -1;
	while (++i < philo->nb_philo)
		set_philosopher(philo, i, ac, av);
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
	philo->nb_philo = ft_atoi(av[1]);
	philo->start = get_time_ms();
	philo->nb_philo_eat = 0;
	philo->is_dead = false;
	philo->monitor_stop = false;
	if (ac == 5)
		philo->monitor_stop = true;						
	philo->philosopher = malloc(sizeof(t_philosopher) * philo->nb_philo);
	if (!philo->philosopher)
	{
		printf(MSG_MALLOC);
		pthread_mutex_destroy(&philo->is_dead_mutex);
		return (true);
	}
	pthread_mutex_init(&philo->monitor_mutex, NULL);
	init_philosopher(philo, ac, av);
	init_forks_mutex(philo);
	return (false);
}
