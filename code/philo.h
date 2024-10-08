/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: machrist <machrist@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:17:33 by machrist          #+#    #+#             */
/*   Updated: 2024/09/02 16:08:06 by machrist         ###   ########.fr       */
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
# define MSG_PTHREAD_MUTEX "Error: pthread_mutex_init failed\n"
# define MSG_PTHREAD_JOIN "Error: pthread_join failed\n"
# define MSG_FORK "has taken a fork\n"
# define MSG_EAT "is eating\n"
# define MSG_SLEEP "is sleeping\n"
# define MSG_THINK "is thinking\n"
# define MSG_DIE "died\n"
# define MSG_PTHREAD_DETACH "Error: pthread_detach failed\n"
# define EAT 0
# define SLEEP 1
# define THINK 2

typedef struct s_philospher
{
	int				id;
	volatile bool	*is_dead;
	bool			finish_eat;
	int				*nb_philo;
	long long		time_to_die;
	long long		time_to_eat;
	long long		time_to_sleep;
	int				nb_eat;
	long long		*start;
	long long		last_meal;
	pthread_t		th;
	pthread_mutex_t	*is_dead_mutex;
	bool			forks_left;
	bool			*forks_right;
	pthread_mutex_t	forks_mutex_left;
	pthread_mutex_t	*forks_mutex_right;
}					t_philosopher;

void				print_msg(t_philosopher *philosopher, char *msg);

typedef struct s_philo
{
	int				nb_philo;
	int				nb_philo_eat;
	long long		start;
	volatile bool	is_dead;
	bool			monitor_stop;
	pthread_t		th;
	t_philosopher	*philosopher;
	pthread_mutex_t	is_dead_mutex;
	pthread_mutex_t	monitor_mutex;
}					t_philo;

int					ft_atoi(const char *str);
bool				init_threads(t_philo *philo);
bool				init_philo(t_philo *philo, int ac, char **av);
void				*routine(void *arg);
long long			get_time_ms(void);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
bool				init_forks_mutex(t_philo *philo);
void				destroy_forks_mutex(t_philo *philo);
void				philosopher_unlock_forks(t_philosopher *philosopher);
void				start_eating(t_philosopher *philosopher);
void				philosopher_lock_forks(t_philosopher *philosopher);
bool				ft_usleep(t_philosopher *philosopher, int act);
bool				check_is_dead(t_philosopher *philosopher,
						long long time_now);
void				*monitor(void *arg);
void				wait_all_threads(t_philo *philo, int i);
bool				validate_arguments(int ac, char **av);
bool				clear_init(t_philo *philo);
bool				clear_fork_mutex(t_philo *philo, int i);

#endif