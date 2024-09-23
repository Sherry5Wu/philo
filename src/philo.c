
#include "philo.h"

static bool	are_all_number(char **args, int arg_amount);

int	main(int ac, char **av)
{
	if (ac == 6 || ac == 7)
	{
		if (are_all_number(av, (ac - 1)))
			init_philos(av, (ac - 1));
		else
			error_msg(NON_NUM_ARG);
		value_check(); // check each passed value is great than 0 or not.
	}
	else if (ac < 6)
		error_msg(TOO_LESS);
	else
		error_msg(TOO_MANY);
	exit(1);
}

/*
	The function will check:
	1. all numbers;
	2. the value of number_of_philosophers, time_to_die,
	   time_to_eat or time_to_sleep  should GREAT than 0;
	3. if there is the value of number_of_times_each_philosopher_must_eat,
	   it should be great than 0.
*/
static bool	are_all_number(char **args, int arg_amount)
{
	int	i;
	int	j;

	i = 0;
	while (i < arg_amount)
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

static void	init_philo(char **args, int arg_amount)
{

}

static bool	value_check()
{

}