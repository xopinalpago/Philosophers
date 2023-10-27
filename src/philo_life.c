/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_life.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmeriau <rmeriau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 11:48:44 by rmeriau           #+#    #+#             */
/*   Updated: 2023/10/16 13:02:38 by rmeriau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	philo_think(t_philo *ph)
{
	long long int	time_to_think;

	if (ph->data->n_philo % 2 == 0)
		time_to_think = (ph->data->time_to_eat - ph->data->time_to_sleep);
	else
		time_to_think = (2 * ph->data->time_to_eat - ph->data->time_to_sleep);
	if (time_to_think < 0)
		time_to_think = 0;
	pthread_mutex_lock(&ph->data->mprint);
	printf("%lld %d is thinking\n", get_time() - ph->data->t0, ph->id);
	pthread_mutex_unlock(&ph->data->mprint);
	if (ft_usleep(time_to_think, ph) == 0)
		return (0);
	return (1);
}

int	philo_forks(t_philo *ph)
{
	if (ph->id % 2 == 0)
		pthread_mutex_lock(ph->right_fork);
	else
		pthread_mutex_lock(ph->left_fork);
	if (!check_death(ph))
		print_fork(ph);
	if (ph->id % 2 == 0)
	{
		if (pthread_mutex_lock(ph->left_fork) != 0)
			return (pthread_mutex_unlock(ph->right_fork), 1);
		if (!check_death(ph))
			print_fork(ph);
	}
	else
	{
		if (pthread_mutex_lock(ph->right_fork) != 0)
			return (pthread_mutex_unlock(ph->left_fork), 1);
		if (!check_death(ph))
			print_fork(ph);
	}
	return (0);
}

int	eat_sleep(t_philo *ph)
{
	if (check_death(ph))
	{
		pthread_mutex_unlock(ph->left_fork);
		pthread_mutex_unlock(ph->right_fork);
		pthread_mutex_unlock(&ph->mlast_meal);
		return (0);
	}
	pthread_mutex_unlock(&ph->mlast_meal);
	if (!ft_usleep(ph->data->time_to_eat, ph))
	{
		pthread_mutex_unlock(ph->left_fork);
		pthread_mutex_unlock(ph->right_fork);
		return (0);
	}
	return (1);
}

int	philo_eat(t_philo *ph)
{
	if (check_death(ph))
		return (0);
	if (philo_forks(ph))
		return (0);
	if (check_death(ph))
	{
		pthread_mutex_unlock(ph->left_fork);
		pthread_mutex_unlock(ph->right_fork);
		return (0);
	}
	pthread_mutex_lock(&ph->mlast_meal);
	pthread_mutex_lock(&ph->data->mprint);
	printf("%lld %d is eating\n", get_time() - ph->data->t0, ph->id);
	pthread_mutex_unlock(&ph->data->mprint);
	ph->last_meal = get_time();
	if (!eat_sleep(ph))
		return (0);
	return (1);
}

int	philo_life(t_philo *ph)
{
	if (!philo_eat(ph))
		return (0);
	pthread_mutex_lock(&ph->m_nom);
	ph->number_of_meals++;
	if (ph->number_of_meals >= ph->data->notepme && ph->data->notepme != -1)
	{
		pthread_mutex_unlock(&ph->m_nom);
		pthread_mutex_unlock(ph->left_fork);
		pthread_mutex_unlock(ph->right_fork);
		return (0);
	}
	pthread_mutex_unlock(&ph->m_nom);
	pthread_mutex_unlock(ph->left_fork);
	pthread_mutex_unlock(ph->right_fork);
	if (check_death(ph))
		return (0);
	pthread_mutex_lock(&ph->data->mprint);
	printf("%lld %d is sleeping\n", get_time() - ph->data->t0, ph->id);
	pthread_mutex_unlock(&ph->data->mprint);
	if (!ft_usleep(ph->data->time_to_sleep, ph))
		return (0);
	if (check_death(ph) || !philo_think(ph))
		return (0);
	return (1);
}
