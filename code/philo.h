/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: machrist <machrist@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:17:33 by machrist          #+#    #+#             */
/*   Updated: 2024/02/29 16:36:37 by machrist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

# define MSG_MALLOC "Error: malloc failed\n"
# define MSG_PTHREAD_CRT "Error: pthread_create failed\n"
# define MSG_PTHREAD_JOIN "Error: pthread_join failed\n"
# define MSG_FORK "has taken a fork\n"
# define MSG_EAT "is eating\n"
# define MSG_SLEEP "is sleeping\n"
# define MSG_THINK "is thinking\n"
# define MSG_DIE "died\n"
# define EAT 0
# define SLEEP 1
# define THINK 2

typedef struct s_philospher
{
	int					id;
	bool				*is_dead;
	int					*fork;
	int					id_left;
	int					id_right;
	unsigned long long	time_to_die;
	unsigned long long	time_to_eat;
	unsigned long long	time_to_sleep;
	int					nb_eat;
	unsigned long long	*start;
	unsigned long long	last_meal;
	pthread_t			th;
	pthread_mutex_t		*is_dead_mutex;
	pthread_mutex_t		*forks_mutex;
}						t_philosopher;

void					print_msg(t_philosopher *philosopher, char *msg);

typedef struct s_philo
{
	int					nb_philo;
	unsigned long long	start;
	bool				is_dead;
	int					*fork;
	t_philosopher		*philosopher;
	pthread_mutex_t		is_dead_mutex;
	pthread_mutex_t		forks_mutex;
}						t_philo;

int						ft_atoi(const char *str);
bool					init_threads(t_philo *philo);
bool					init_philo(t_philo *philo, int ac, char **av);
void					*routine(void *arg);
unsigned long long		get_time_ms(void);
int						ft_strncmp(const char *s1, const char *s2, size_t n);

#endif