/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jingwu <jingwu@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 10:27:36 by jingwu            #+#    #+#             */
/*   Updated: 2024/11/04 10:27:36 by jingwu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	contains_only_digits(char **args)
{
	int	i;
	int	j;

	i = 0;
	while (args[i])
	{
		j = 0;
		while (args[i][j])
		{
			if (args[i][j] < 48 || args[i][j] > 57)
				return (false);
			j++;
		}
		i++;
	}
	return (true);
}

static bool	check_range(char **args)
{
	if (ft_atoi(args[1]) <= 0)
		return (error_msg(PHILO_NUM_ERR));
	if (ft_atoi(args[2]) < 0)
		return (error_msg(TIME_DIE_ERR));
	if (ft_atoi(args[3]) <= 0)
		return (error_msg(TIME_EAT_ERR));
	if (ft_atoi(args[4]) < 0)
		return (error_msg(TIME_SLEEP_ERR));
	if (args[5] && ft_atoi(args[5]) <= 0)
		return (error_msg(MUST_EST_ERR));
	return (true);
}

/*
	The function will check:
	1. are all the characters in the argments numbers;
	2. validate the value of the argments are in the right range:
		2.1  the value of number_of_philosophers, time_to_die,
		     time_to_eat or time_to_sleep  should GREAT than 0;
		2.2  if there is the value of number_of_times_each_philosopher_must_eat,
		     it should be great than 0.
*/
static bool	validate_args(int arg_amount, char **args)
{
	int	i;

	i = 1;
	while (i < arg_amount)
	{
		if (!contains_only_digits(args))
			return (error_msg(NON_NUM_ARG));
		if (!check_range(args))
			return (false);
	}
	return (true);
}

/*
	The function:
	Launches the simulation by creating a grim reaper thread as well as
	one thread for each philosopher.

	Why start time = current time + (philo_nb * 10 * 2)?
	To give a little bit extra time for the program to create thread for
	every philosophers. Try to let every phiolsopher thread starts at
	same time.

	@return
	0 if the simulation was successfully started;
	1 if there was an error.
*/
static int	start_simulation(t_table *table)
{
	size_t	i;

	table->start_time = get_time_in_ms() + (table->philo_nb * 10 * 2);
	i = 0;
	while (i < table->philo_nb)
	{
		if (pthread_create(&table->philos[i]->thread, NULL,
			&routine, table->philos[i]) != 0)
		{
			clean_threads(table, i);
			return (error_msg(CREATE_THREAD_ERR));
		}
		i++;
	}
	monitor(table);
	return (0);
}

int	main(int ac, char **av)
{
	t_table	table;

	if (ac == 5 || ac == 6)
	{
		if(!validate_args(ac, av))
			return (EXIT_FAILURE);
		// table = malloc(sizeof(t_table));
		// if (!table)
		// 	return(error_msg(MALLOC_ERR));
		if (init_table(&table, ac, av) == false)
			return(stop_simulation(&table));
		if (start_simulation(&table) == 1)
			return(stop_simulation(&table));
		stop_simulation(&table);
		return (EXIT_SUCCESS);
	}
	else if (ac < 5)
		putstr_fd(TOO_LESS, 2);
	else
		putstr_fd(TOO_MANY, 2);
	return (EXIT_FAILURE);
}
