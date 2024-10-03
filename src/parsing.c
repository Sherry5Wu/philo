
#include "philo.h"

static bool	contains_only_digits(char **args)
{
	int	i;
	int	j;

	i = 0;
	while (arg[i])
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
	if (ft_atoi(args[1]) <= 0 || ft_atoi(args[1] > PHILO_MAX))
		return (putstr_fd(PHILO_NUM_ERR, 2))
	if (ft_atoi(args[2]) <= 0)
		return (putstr_fd(TIME_DIE_ERR, 2));
	if (ft_atoi(args[3]) <= 0)
		return (putstr_fd(TIME_EAT_ERR, 2));
	if (ft_atoi(args[4]) <= 0)
		return (putstr_fd(TIME_SLEEP_ERR, 2));
	if (args[5] && ft_atoi(args[5]) <= 0)
		return (putstr_fd(MUST_EST_ERR, 2));
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
bool	validate_args(int arg_amount, char **args)
{
	int	i;
	int	nbr;

	i = 1;
	while (i < arg_amount)
	{
		if (!contains_only_digits(args[i]))
			return (putstr_fd(NON_NUM_ARG, 2););
		if (!check_range(args))
			return (false);
	}
	return (true);
}


