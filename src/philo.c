
#include "philo.h"

int	main(int ac, char **av)
{
	t_table	*table;

	if (ac == 5 || ac == 6)
	{
		if(!validate_args(ac, av)) // Done
			return (EXIT_FAILURE);
		table = init_table(ac, av, 1)// underway
		if (!table)
			return (EXIT_FAILURE);

	}
	else if (ac < 5)
		putstr_fd(TOO_LESS, 2);
	else
		putstr_fd(TOO_MANY, 2);
	return (1);
}

