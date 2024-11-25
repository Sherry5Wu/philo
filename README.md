# philo

# functions

## thread

	1. pthread_create
	Purpose: Creates a new thread and specifies the function that the thread will execute.

	Prototype:
	int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg);

	thread: A pointer to a pthread_t variable that stores the thread ID.

	attr: Thread attributes, which can be NULL to use default attributes.

	start_routine: The function the thread will execute. This function must take a void* argument and return a void*.

	arg: The argument passed to start_routine. If not needed, it can be NULL.

	Return Value:

	Returns 0 on success.

	Returns an error code on failure.

	Example:

		#include <pthread.h>
		#include <stdio.h>

		void* print_message(void* arg) {
		    printf("Hello from thread!\n");
		    return NULL;
		}

		int main() {
		    pthread_t thread;
		    pthread_create(&thread, NULL, print_message, NULL);
		    pthread_join(thread, NULL);  // 等待线程结束
		    return 0;
		}

	2. pthread_detach
	Purpose: Sets the thread to a detached state. Detached threads automatically release their resources

 	when they terminate, and do not require pthread_join.

	Prototype:
	int pthread_detach(pthread_t thread);

	thread: The thread ID of the thread to be detached.

	Return Value:

	Returns 0 on success.

	Returns an error code on failure.

	Note: After calling pthread_detach, you cannot use pthread_join on the thread.

	3. pthread_join

	Purpose: Waits for a thread to terminate and optionally retrieves the thread’s return value.

	Prototype:

	int pthread_join(pthread_t thread, void **retval);

	thread: The thread ID of the thread to wait for.

	retval: A pointer to store the thread’s return value. If not needed, pass NULL.

	Return Value:

	Returns 0 on success.

	Returns an error code on failure.

	Note: It is important to call pthread_join for non-detached threads to avoid resource leaks.

# how to assign forks for philosopher

The fork assignment logic in the classic philosophers problem is designed to ensure that each philosopher can safely pick up the necessary two forks without causing deadlocks. Here's a breakdown of the logic used in the function you provided:

static void	assign_forks(t_philo *philo)
{
	philo->fork[0] = philo->id;
	philo->fork[1] = (philo->id + 1) % philo->table->nb_philos;
	if (philo->id % 2)
	{
		philo->fork[0] = (philo->id + 1) % philo->table->nb_philos;
		philo->fork[1] = philo->id;
	}
}

Key Concepts:
philo->id: The unique ID of each philosopher.

philo->table->nb_philos: The total number of philosophers (and forks).

fork[0] and fork[1]: Represent the left and right forks that a philosopher must pick up to eat.

Fork Assignment Logic:

Initial Assignment:

The first assignment assumes that each philosopher has a fork on their left (fork[0] = philo->id) and a fork on their right (fork[1] = (philo->id + 1) % nb_philos).
This means that a philosopher's left fork has the same number as their id, and their right fork is the next fork in sequence (with wrapping around for the last philosopher using modulus %).
Special Case for Odd-Numbered Philosophers:

For philosophers with an odd id, the forks are swapped: they pick up their right fork first (fork[0] = (philo->id + 1) % nb_philos) and then their left fork (fork[1] = philo->id).
This swap helps prevent deadlocks, where all philosophers might grab the left fork at the same time, causing them to wait indefinitely for the right fork.
Why the Total Number of Philosophers Affects Fork Distribution:
The modulo operation (philo->id + 1) % nb_philos ensures that the philosopher seated last (with id = nb_philos - 1) wraps around and shares a fork with the philosopher seated first (with id = 0). This circular arrangement means that all forks are shared between two adjacent philosophers.
For odd-numbered philosophers, swapping the fork order (picking up the right one first) helps avoid scenarios where all philosophers might attempt to grab the same fork at the same time, reducing the likelihood of deadlock.
Example:
Let’s consider a table with 4 philosophers:

Philosopher 0: Left fork 0, Right fork 1.

Philosopher 1: Left fork 2, Right fork 1 (due to the odd id, forks are swapped).

Philosopher 2: Left fork 2, Right fork 3.

Philosopher 3: Left fork 0, Right fork 3 (again, swapped due to odd id).

Avoiding Deadlocks:
The key insight here is that by alternating the order in which forks are picked up, odd-numbered philosophers grab the right fork first. This reduces the chance of a deadlock, where every philosopher waits indefinitely for their neighbor to release a fork.

Conclusion:
The total number of philosophers and the fork assignment logic both play crucial roles in ensuring that the philosophers can eat without causing a deadlock. The alternating strategy for odd-numbered philosophers is a clever way to prevent resource contention and allow for smooth execution of the dining philosophers' problem.


# Testing tools

## Philosopher visualizer

https://nafuka11.github.io/philosophers-visualizer/


