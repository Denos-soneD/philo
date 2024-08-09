/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: machrist <machrist@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:26:07 by machrist          #+#    #+#             */
/*   Updated: 2024/08/06 18:17:22 by machrist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>

static long int	ft_strtol(const char *nptr)
{
	long int	res;
	int			i;
	int			s;

	res = 0;
	i = 0;
	s = 1;
	while (nptr[i] == ' ' || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
		if (nptr[i++] == '-')
			s = -1;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		if (res != (res * 10 + nptr[i] - '0') / 10)
		{
			if (s == -1)
				return (LONG_MIN);
			return (LONG_MAX);
		}
		res = res * 10 + nptr[i++] - '0';
	}
	return (res * s);
}

int	ft_atoi(const char *str)
{
	return ((int)ft_strtol(str));
}
