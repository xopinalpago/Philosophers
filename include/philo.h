/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmeriau <rmeriau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 16:22:49 by rmeriau           #+#    #+#             */
/*   Updated: 2023/10/17 12:22:36 by rmeriau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philo
{
	int				id;
	long long int	last_meal;
	pthread_mutex_t	mlast_meal;
	int				number_of_meals;
	pthread_mutex_t	m_nom;
	pthread_t		thread_id;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	struct s_table	*data;
}					t_philo;

typedef struct s_table
{
	int				n_philo;
	long long int	time_to_die;
	long long int	time_to_eat;
	long long int	time_to_sleep;
	long long int	t0;
	long long int	notepme;
	int				dead;
	pthread_t		*threads;
	pthread_mutex_t	*forks;
	pthread_mutex_t	mdead;
	pthread_mutex_t	mprint;
	t_philo			*philos;
}					t_table;

# ifndef INT_MAX
#  define INT_MAX 2147483647
# endif

int					init_forks(t_table *data);
int					init_philos(t_table *data);
int					is_valid(char *nb);
int					check_numbers(int argc, char *argv[], t_table *data);
int					init_table(t_table *data);

long long int		get_time(void);
int					ft_usleep(long long int ms, t_philo *ph);
void				print_fork(t_philo *ph);
void				free_data(t_table *data);
long long int		ft_long_atoi(const char *str, int *good);

int					check_death(t_philo *ph);
int					test_dead(t_table *data, int i);
void				is_dead(t_table *data);

int					philo_think(t_philo *ph);
int					philo_forks(t_philo *ph);
int					philo_eat(t_philo *ph);
int					philo_life(t_philo *ph);

void				*do_philo(void *philo);
int					ft_creation(t_table *data);
void				ft_destroy(t_table *data);
int					ft_simulation(t_table *data);

#endif