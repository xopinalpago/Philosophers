/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmeriau <rmeriau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 18:14:28 by rmeriau           #+#    #+#             */
/*   Updated: 2023/10/17 10:40:25 by rmeriau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	check_death(t_philo *ph)
{
	int	res;

	pthread_mutex_lock(&ph->data->mdead);
	if (ph->data->dead)
		res = 1;
	else
		res = 0;
	pthread_mutex_unlock(&ph->data->mdead);
	return (res);
}

int	test_dead(t_table *data, int i)
{
	pthread_mutex_lock(&data->philos[i].mlast_meal);
	if (get_time() - data->philos[i].last_meal >= data->time_to_die)
	{
		pthread_mutex_unlock(&data->philos[i].mlast_meal);
		pthread_mutex_lock(&data->mprint);
		printf("%lld %d died\n", get_time() - data->t0,
			data->philos[i].id);
		pthread_mutex_unlock(&data->mprint);
		pthread_mutex_lock(&data->mdead);
		data->dead = 1;
		pthread_mutex_unlock(&data->mdead);
		return (1);
	}
	pthread_mutex_unlock(&data->philos[i].mlast_meal);
	return (0);
}

void	is_dead(t_table *data)
{
	int	i;

	i = 0;
	while (1)
	{
		usleep(100);
		i = 0;
		while (i < data->n_philo)
		{
			pthread_mutex_lock(&data->philos[i].m_nom);
			if (data->philos[i].number_of_meals >= data->notepme
				&& data->notepme != -1)
			{
				pthread_mutex_unlock(&data->philos[i].m_nom);
				return ;
			}
			pthread_mutex_unlock(&data->philos[i].m_nom);
			if (test_dead(data, i))
				return ;
			i++;
		}
	}
	return ;
}
