
#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>

// Error_messages define
# define TOO_LESS		"Too less argments. Shoud pass at least 5 arguments"
# define TOO_MANY		"Too many arguments. Can pass 6 arguments at most."
# define NON_NUM_ARG	"Non-number characters. All the arguments should just contain numbers."

typedef struct s_philo
{
	size_t	philo_amount;
	size_t	time_to_die;
	size_t	time_to_eat;
	size_t	time_to_sleep;
	int		each_eat_times;
}	t_philo;


// utils.c
void	error_msg(char	*message);

# endif