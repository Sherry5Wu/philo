/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jingwu <jingwu@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 14:48:37 by jingwu            #+#    #+#             */
/*   Updated: 2024/11/19 14:48:37 by jingwu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <sys/time.h>

/*-----------------------------Define Error Messages------------------------*/
# define TOO_LESS		"Too less argments. Should pass at least 5 arguments"
# define TOO_MANY		"Too many arguments. Can pass 6 arguments at most."
# define NON_NUM_ARG	"Non-number characters.All the arguments should \
just contain numbers."
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
# define FAIL_JOIN_THR	"error: Failed to join a thread."

/*----------------------------------Structs--------------------------------*/
typedef struct s_table	t_table;

typedef enum e_action
{
	THINKING = 0,
	EATING = 1,
	SLEEPING = 2,
	GOT_LEFT_FORK = 3,
	GOT_RIGHT_FORK = 4,
	DIED,
}	t_action;

typedef enum e_state
{
	READY = 0,
	FULL,
	OVER,
}	t_state;

typedef struct s_philo
{
	pthread_t		thread;
	size_t			id;
	size_t			meals_eaten;
	time_t			last_eat;
	pthread_mutex_t	philo_lock;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
	t_table			*table;
	t_state			state;
	t_action		action;
}	t_philo;

typedef struct s_table
{
	time_t			start_time;
	size_t			philo_nb;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	int				must_eat_times;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	*fork_locks;
	t_philo			*philos;
}	t_table;

/*----------------------------------Functions-------------------------------*/
// init_table.c
bool	init_table(t_table *table, int ac, char **args);

// libft.c
int		ft_strlen(char *str);
void	putstr_fd(char	*message, int fd);
int		ft_atoi(const char *str);

// monitor.c
void	monitor(t_table *table);

//routine_actions.c
bool	philo_eating_sleeping(t_philo *philo);
// bool	philo_sleeping(t_philo *philo);
bool	philo_thinking(t_philo *philo);

// routine.c
void	*routine(void *data);
bool	thread_sleep(t_philo *philo, size_t duration);
void	update_philo_state(t_philo *philo, int state);
int		get_philo_state(t_philo *philo);
void	put_down_forks(t_philo *philo);

// stop_simulation.c
void	clean_threads(t_table *table, size_t thread_nb);
void	destroy_mutexes_of_table(t_table *table);
void	free_table(t_table *table);
int		stop_simulation(t_table *table);

// utils.c
time_t	get_time_in_ms(void);
void	ft_free(void *str);
bool	error_msg(char	*message);
void	*error_msg_null(char *message);
void	print_philo_action_msg(t_philo *philo, t_action action);

#endif
