/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmeriau <rmeriau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 14:47:06 by rmeriau           #+#    #+#             */
/*   Updated: 2023/10/17 12:22:00 by rmeriau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	init_forks(t_table *data)
{
	int	i;

	i = 0;
	while (i < data->n_philo)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			return (0);
		i++;
	}
	return (1);
}

int	init_philos(t_table *data)
{
	int	i;

	i = 0;
	while (i < data->n_philo)
	{
		data->philos[i].id = i + 1;
		data->philos[i].data = data;
		data->philos[i].number_of_meals = 0;
		data->philos[i].last_meal = get_time();
		data->philos[i].left_fork = &data->forks[i];
		if (i == 0)
			data->philos[i].right_fork = &data->forks[data->n_philo - 1];
		else
			data->philos[i].right_fork = &data->forks[i - 1];
		if (pthread_mutex_init(&data->philos[i].mlast_meal, NULL) != 0)
			return (0);
		if (pthread_mutex_init(&data->philos[i].m_nom, NULL) != 0)
			return (0);
		i++;
	}
	return (1);
}

int	is_valid(char *nb)
{
	int	i;

	i = 0;
	if (nb[i] == '+' && nb[i + 1])
	{
		if (nb[i + 1])
			i++;
		else
			return (0);
	}
	while (nb[i])
	{
		if (nb[i] < '0' || nb[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	check_numbers(int argc, char *argv[], t_table *data)
{
	int	good;

	good = 1;
	if (!is_valid(argv[1]) || !is_valid(argv[2])
		|| !is_valid(argv[3]) || !is_valid(argv[4]))
		return (0);
	data->n_philo = ft_long_atoi(argv[1], &good);
	data->time_to_die = ft_long_atoi(argv[2], &good);
	data->time_to_eat = ft_long_atoi(argv[3], &good);
	data->time_to_sleep = ft_long_atoi(argv[4], &good);
	if (argc == 6)
	{
		if (!is_valid(argv[5]))
			return (0);
		else
			data->notepme = ft_long_atoi(argv[5], &good);
	}
	else
		data->notepme = -1;
	if (good == 0)
		return (0);
	return (1);
}

int	init_table(t_table *data)
{
	data->dead = 0;
	data->threads = malloc(sizeof(pthread_t) * data->n_philo);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->n_philo);
	data->philos = malloc(sizeof(t_philo) * data->n_philo);
	if (!data->threads || !data->forks || !data->philos)
		return (0);
	if (pthread_mutex_init(&data->mdead, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&data->mprint, NULL) != 0)
		return (0);
	return (1);
}
