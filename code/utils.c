/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: machrist <machrist@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 01:25:36 by machrist          #+#    #+#             */
/*   Updated: 2024/08/10 01:50:07 by machrist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	wait_all_threads(t_philo *philo, int i)
{
	pthread_join(philo->th, NULL);
	while (i--)
		pthread_join(philo->philosopher[i].th, NULL);
}
