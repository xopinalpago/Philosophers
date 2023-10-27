/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmeriau <rmeriau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 17:08:28 by rmeriau           #+#    #+#             */
/*   Updated: 2023/10/17 10:43:41 by rmeriau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

long long int	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	ft_usleep(long long int ms, t_philo *ph)
{
	long long int	nttd;
	long long int	new_ms;

	pthread_mutex_lock(&ph->m_nom);
	nttd = ph->data->t0 + (ph->number_of_meals + 1) * ph->data->time_to_die;
	pthread_mutex_unlock(&ph->m_nom);
	new_ms = nttd - get_time();
	if (new_ms < 0)
	{
		return (0);
	}
	else if (new_ms > ms)
	{
		if (usleep(ms * 1000) == -1)
			return (0);
	}
	else
	{
		usleep(new_ms * 1000);
		if (check_death(ph))
			return (0);
		usleep((ms - new_ms) * 1000);
	}
	return (1);
}

void	free_data(t_table *data)
{
	if (data->threads)
		free(data->threads);
	if (data->forks)
		free(data->forks);
	if (data->philos)
		free(data->philos);
}

void	print_fork(t_philo *ph)
{
	pthread_mutex_lock(&ph->data->mprint);
	printf("%lld %d has taken a fork\n", get_time() - ph->data->t0, ph->id);
	pthread_mutex_unlock(&ph->data->mprint);
}

long long int	ft_long_atoi(const char *str, int *good)
{
	int			i;
	long long	neg;
	long long	n;
	long long	save;

	i = 0;
	neg = 1;
	n = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			neg *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		save = n;
		n = n * 10 + (str[i] - 48);
		i++;
		if ((save <= n && n == 0) || n * neg < 0 || n > INT_MAX)
			*good = 0;
	}
	return (n * neg);
}
