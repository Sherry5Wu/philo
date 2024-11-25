/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stop_simulation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jingwu <jingwu@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 13:53:39 by jingwu            #+#    #+#             */
/*   Updated: 2024/11/25 08:38:40 by jingwu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_table(t_table *table)
{
	size_t	i;

	if (!table)
		return ;
	if (table->fork_locks)
		ft_free(table->fork_locks);
	if (table->philos)
	{
		i = 0;
		while (i < table->philo_nb)
		{
			if (table->philos[i])
				ft_free(table->philos[i]);
			i++;
		}
		ft_free(table->philos);
	}
}

/*
	@function
	It is used when there is an error during the simulation or when the
	simulation is stopped.
	Set all philosophers' status to DIED, and close all the threads.
*/
void	clean_threads(t_table *table, size_t thread_nb)
{
	size_t	i;

	i = 0;
	// printf("thread_nb=%zu\n", thread_nb);//for testing
	while (i < thread_nb)
	{
		set_philo_status(table->philos[i], DIED);
		i++;
	}
	while (i < thread_nb)
	{
		// printf("i=%zu\n", i);//for testing
		if (pthread_join(table->philos[i]->thread, NULL) != 0)
		{
			pthread_mutex_lock(&table->write_lock);
			error_msg(FAIL_JOIN_THR);
			pthread_mutex_unlock(&table->write_lock);
		}
		i++;
	}
}
/*
	@function
	Destroy all the mutexes which belong to the table.
	Called when errors are generated in initializing philos.
*/
void	destroy_mutexes_of_table(t_table *table)
{
	size_t	i;

	i = 0;
	if (!table)
		return ;
	while (i < table->philo_nb)
	{
		pthread_mutex_destroy(&table->fork_locks[i]);
		i++;
	}
	pthread_mutex_destroy(&table->sim_stop_lock);
	pthread_mutex_destroy(&table->write_lock);
}
/*
	@function
	Destroy all the mutexes of the whole process.
	Called when the simulation already starts, then some error are generated;
	or when the simulation stops.
*/
static void	destroy_all_mutexes(t_table *table)
{
	size_t	i;

	i = 0;
	if (!table)
		return ;
	while (i < table->philo_nb)
	{
		pthread_mutex_destroy(&table->fork_locks[i]);
		pthread_mutex_destroy(&table->philos[i]->philo_lock);
		i++;
	}
	pthread_mutex_destroy(&table->sim_stop_lock);
	pthread_mutex_destroy(&table->write_lock);
}

/*
	@function
	It is used when the simulation stopped.
	It will destroy all the mutexes, clean all the threads and free all the
	memories.
*/
int	stop_simulation(t_table *table)
{
	destroy_all_mutexes(table);
	free_table(table);
	return (EXIT_FAILURE);
}
