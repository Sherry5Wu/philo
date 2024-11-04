
#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <sys/time.h>

# ifndef DEBUG_FORMAT
# define DEBUG_FORMAT	(0)
# endif

// Error_messages define
# define TOO_LESS		"Too less argments. Shoud pass at least 5 arguments"
# define TOO_MANY		"Too many arguments. Can pass 6 arguments at most."
# define NON_NUM_ARG	"Non-number characters. All the arguments should just contain numbers."
# define PHILO_NUM_ERR	"Invalid philosopher number."
# define TIME_DIE_ERR	"Invalid time to die."
# define TIME_EAT_ERR	"Invalid time to eat."
# define TIME_SLEEP_ERR	"Invalid time to sleep."
# define MUST_EST_ERR	"Invalid number of times each philosopher must eat"
# define MALLOC_ERR		"error: Could not allocate memory."
# define CREATE_MUTEX_ERR	"error: Could not create mutex."
# define CREATE_THREAD_ERR	"error: Could not create thread."
# define INIT_PHILO_ERR	"Initialized philos failed."
# define INIT_MUTEX_ERR	"Initialized mutex failed."
# define GET_TIME_ERR	"error: gettimeofday failed."


typedef struct s_table
{
	time_t			start_time;
	size_t			philo_nb;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	int				each_eat_times;
	pthread_t		monitor;
	bool			simulation_stop;
	pthread_mutex_t	sim_stop_lock;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	*fork_locks;
	t_philo			**philos;
}	t_table;

typedef struct s_philo
{
	pthread_t		thread;
	size_t			id;
	size_t			meals_eaten;
	time_t			last_eat;
	pthread_mutex_t	meal_time_lock;
	size_t			fork[2];
	t_table			*table;
}	t_philo;

typedef enum e_status
{
	THINKING = 0,
	EATING = 1,
	SLEEPING = 2,
	DIED = 3,
	GOT_LEFT_FORK = 4,
	GOT_RIGHT_FORK = 5,
	FULL = 6
}	t_status;


// parsing.c
bool	validate_args(int arg_amount, char **args);

// libft.c
bool	putstr_fd(char	*message, int fd);
int		ft_atoi(const char *str);
int		ft_strlen(char *str);

// exit.c
void	*error_null(char *str, t_table *table);
void	*free_table(t_table *table);

// rountine.c
void	*routine(void *data);

// utils.c
void	error_msg(char	*message);
time_t	get_time_in_ms(void);
void	print_status_msg(t_philo *philo, bool stop_sign, t_status status);

// monitor.c
bool	is_simulation_stopped(t_table *table);

#endif

//https://github.com/mcombeau/philosophers/blob/main/philo/includes/philo.h
