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

static bool	are_valid_numbers(char **args)
{
	int	i;
	int	j;

	i = 1;
	while (args[i])
	{
		j = 0;
		while (args[i][j])
		{
			if (args[i][j] < 48 || args[i][j] > 57)
			{
				if(args[i][j] != '-' && args[i][j] != '+')
					return (false);
				else
				{
					if (args[i][j + 1] < 48 || args[i][j + 1] > 57)
						return (false);
				}
			}
			j++;
		}
		i++;
	}
	return (true);
}

/*
	@return
	0: all the values are in the range;
	1: there is at least one value is not in the correct range.
*/
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
static bool	validate_args(char **args)
{
	if (are_valid_numbers(args) == false)
		return (error_msg(NON_NUM_ARG));
	if (check_range(args) == false)
		return (false);
	return (true);
}

/*
	The function:
	Launches the simulation by creating a grim reaper thread as well as
	one thread for each philosopher.

	@return
	0 if the simulation was successfully started;
	1 if there was an error.
*/
static bool	start_simulation(t_table *table)
{
	size_t	i;

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
	return (true);
}

// void print_table(t_table table)
// {
// 	printf("nb=%zu\n",table.philo_nb);
// 	printf("time_die=%zu\n",table.time_to_die);
// 	printf("time_eat=%zu\n",table.time_to_eat);
// 	printf("time_sleep=%zu\n",table.time_to_sleep);
// 	printf("must_eat=%d\n",table.must_eat_times);
// 	printf("simulation_stop=%d\n",table.simulation_stop);
// }


int	main(int ac, char **av)
{
	t_table	table;

	if (ac == 5 || ac == 6)
	{
		if(validate_args(av) == false)
			return (EXIT_FAILURE);
		if (init_table(&table, ac, av) == false)
		{
			free_table(&table);
			return (EXIT_FAILURE);
		}
		if (start_simulation(&table) == false)
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
// valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --tool=helgrind ./philo

