/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philos.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jingwu <jingwu@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 13:38:01 by jingwu            #+#    #+#             */
/*   Updated: 2024/11/21 10:24:26 by jingwu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
/*
	Using (id+1)%(nb_philo) will nicely make a cycle.
	The logic to assign forks: (consider the left fork is their own fork)
	-	the odd id philo will first pick the fork on their right, then pick up
		the fork on the left (their own fork);
	-	the even id philo will first pick the for on their left(their own fork)
		then pick the fork on the right.
*/
static void	assign_forks(pthread_mutex_t *forks, t_philo *philo)
{
	if (philo->id % 2)
	{
		philo->l_fork = &forks[(philo->id + 1) % (philo->table->philo_nb)];
		philo->r_fork = &forks[philo->id];
	}
	else
	{
		philo->l_fork = &forks[philo->id];
		philo->r_fork = &forks[(philo->id + 1) % (philo->table->philo_nb)];
	}
}
static void	free_philos(t_philo **philos, int i)
{
	while (--i >= 0)
		ft_free(philos[i]);
	ft_free(philos);
}

static bool	init_philo_mutex(t_philo **philos, int i)
{
	if (pthread_mutex_init(&philos[i]->philo_lock, 0) != 0)
	{
		while(--i >= 0)
			pthread_mutex_destroy(&philos[i]->philo_lock);
		return (error_msg_null(CREATE_MUTEX_ERR));
	}
	return (true);
}
static void	init_last_eat_time(t_philo *philo)
{
	pthread_mutex_lock(&philo->philo_lock);
	philo->last_eat = philo->table->start_time;
	// printf("last_eat=%ld\n", philo->last_eat);// for testing!!!!!!!
	pthread_mutex_unlock(&philo->philo_lock);
}

/*
	pthread_mutex_init() return:
	0: successfully;
	others: failed;
*/
t_philo	**init_philos(t_table *table)
{
	t_philo			**philos;
	int				i;

	philos = malloc(sizeof(t_philo *) * (table->philo_nb));
	if (!philos)
		return (error_msg_null(MALLOC_ERR));
	i = -1;
	while (++i < (int)table->philo_nb)
	{
		philos[i] = malloc(sizeof(t_philo));
		if (!philos[i])
		{
			free_philos(philos, i);
			return (error_msg_null(MALLOC_ERR));
		}
		if (init_philo_mutex(philos, i) == false)
			return (NULL);
		philos[i]->table = table;
		philos[i]->id = i + 1;
		philos[i]->meals_eaten = 0;
		philos[i]->status = THINKING;
		assign_forks(table->fork_locks, philos[i]);
		init_last_eat_time(philos[i]);
	}
	return (philos);
}
