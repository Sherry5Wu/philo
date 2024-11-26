/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jingwu <jingwu@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 13:38:56 by jingwu            #+#    #+#             */
/*   Updated: 2024/11/20 13:38:56 by jingwu           ###   ########.fr       */
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
static void	assign_forks(pthread_mutex_t *forks, t_philo *philo, size_t i)
{
	philo->l_fork = &forks[i];
	if (i == 0)
		philo->r_fork = &forks[philo->table->philo_nb - 1];
	else
		philo->r_fork = &forks[i - 1];
}

/*
	pthread_mutex_init() return:
	0: successfully;
	others: failed;
*/
static bool	init_philos(t_table *table)
{
	size_t	i;
	t_philo	*philo;

	i = 0;
	while (i < table->philo_nb)
	{
		philo = table->philos + i;
		if (pthread_mutex_init(&philo->philo_lock, NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&philo->philo_lock);
			return (error_msg(CREATE_MUTEX_ERR));
		}
		philo->table = table;
		philo->id = i + 1;
		philo->meals_eaten = 0;
		philo->action = THINKING;
		philo->state = ALIVE;
		pthread_mutex_lock(&philo->philo_lock);
		philo->last_eat = philo->table->start_time;
		pthread_mutex_unlock(&philo->philo_lock);
		assign_forks(table->fork_locks, philo, i);
		i++;
	}
	return (true);
}

/*
	@function
	Allocate memory for the fork mutex and initialization them.

	@return
	return a poninter to the fork mutex array,
	or NULL if an error occurd.
*/
static pthread_mutex_t	*init_forks(t_table *table)
{
	pthread_mutex_t	*forks;
	int				i;

	i = 0;
	forks = malloc(sizeof(pthread_mutex_t) * table->philo_nb);
	if (!forks)
		return (NULL);
	while (i < (int)table->philo_nb)
	{
		if (pthread_mutex_init(&forks[i], 0) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&forks[i]);
			ft_free(forks);
			forks = NULL;
			return (NULL);
		}
		i++;
	}
	return (forks);
}

/*
	@function
	Initializes mutex locks for forks, writing and the stop simulation flag.

	@reurn
	true if the initalizations were successful;
	false if initilization failed;
*/
static bool	init_mutexes_for_table(t_table *table)
{
	int	i;

	i = -1;
	table->fork_locks = init_forks(table);
	if (!table->fork_locks)
		return (false);
	if (pthread_mutex_init(&(table->write_lock), 0) != 0)
	{
		while (++i < (int)table->philo_nb)
			pthread_mutex_destroy(&table->fork_locks[i]);
		return (false);
	}
	return (true);
}

/*
	@The function does:
	Initializes the "dining table", the data structure containing
	all of the program's parameters.

	@return
	Returns a pointer to the allocated table structure, or NULL if
	an error occured during initialization.
*/
bool	init_table(t_table *table, int ac, char **args)
{
	table->philo_nb = (size_t)ft_atoi(args[1]);
	table->time_to_die = (size_t)ft_atoi(args[2]);
	table->time_to_eat = (size_t)ft_atoi(args[3]);
	table->time_to_sleep = (size_t)ft_atoi(args[4]);
	table->must_eat_times = -1;
	if (ac - 1 == 5)
		table->must_eat_times = (size_t)ft_atoi(args[5]);
	table->start_time = get_time_in_ms();
	if (!init_mutexes_for_table(table))
		return (error_msg(INIT_MUTEX_ERR));
	table->philos = malloc(sizeof(t_philo) * (table->philo_nb));
	if (!table->philos)
		return (error_msg_null(MALLOC_ERR));
	if (init_philos(table) == false)
	{
		destroy_mutexes_of_table(table);
		return (error_msg(INIT_PHILO_ERR));
	}
	return (true);
}
