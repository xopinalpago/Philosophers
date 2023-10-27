/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmeriau <rmeriau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 12:11:33 by rmeriau           #+#    #+#             */
/*   Updated: 2023/10/17 11:18:03 by rmeriau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	*do_philo(void *philo)
{
	t_philo	*ph;

	ph = (t_philo *)philo;
	if (ph->data->n_philo == 1)
		return (NULL);
	if (ph->id % 2 == 0)
		if (ft_usleep(ph->data->time_to_eat, ph) == 0)
			return (NULL);
	while (1)
	{
		if (philo_life(ph) == 0)
			return (NULL);
	}
	return (NULL);
}

int	ft_creation(t_table *data)
{
	int	i;

	data->t0 = get_time();
	i = 0;
	while (i < data->n_philo)
	{
		if (pthread_create(&data->threads[i], NULL, &do_philo,
				&data->philos[i]))
		{
			return (0);
		}
		i++;
	}
	return (1);
}

void	ft_destroy(t_table *data)
{
	int	i;

	i = 0;
	while (i < data->n_philo)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&data->philos[i].mlast_meal);
		pthread_mutex_destroy(&data->philos[i].m_nom);
		i++;
	}
	pthread_mutex_destroy(&data->mdead);
	pthread_mutex_destroy(&data->mprint);
}

int	ft_simulation(t_table *data)
{
	int	i;

	if (!ft_creation(data))
		return (free_data(data), 0);
	is_dead(data);
	i = 0;
	while (i < data->n_philo)
	{
		pthread_join(data->threads[i], NULL);
		i++;
	}
	ft_destroy(data);
	free_data(data);
	return (1);
}

int	main(int argc, char *argv[])
{
	t_table	data;

	if (argc >= 5 && argc <= 6)
	{
		if (!check_numbers(argc, argv, &data))
			return (1);
		if (!init_table(&data))
			return (free_data(&data), 1);
		if (!init_philos(&data) || !init_forks(&data))
			return (free_data(&data), 1);
		if (!ft_simulation(&data))
			return (1);
		return (0);
	}
	return (1);
}
