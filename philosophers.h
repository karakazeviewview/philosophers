/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmatsuo <mmatsuo@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 00:12:51 by mmatsuo           #+#    #+#             */
/*   Updated: 2023/02/09 19:09:21 by mmatsuo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

# define MAX_PHILO 200

# define STATE_FORK "has taken a fork"
# define STATE_EAT "is eating"
# define STATE_SLEEP "is sleeping"
# define STATE_THINK "is thinking"
# define STATE_DIED "died"

# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <sys/time.h>
# include <unistd.h>

# define ERROR_ARG_INVALID "Error, invalid arg"

typedef struct s_philo
{
	int				id;
	int				num_of_eaten;
	time_t			time_last_eat;
	pthread_mutex_t	*fork_left;
	pthread_mutex_t	*fork_right;
	struct s_data	*data;
	pthread_t		thread;
	long			time_start;
}	t_philo;

typedef struct s_data
{
	long			num_philo;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			philo_must_eat;

	int				dead_num;

	pthread_mutex_t	philo_mtx[MAX_PHILO];
	pthread_mutex_t	fork_mtx[MAX_PHILO];
	pthread_mutex_t	write_finish_mtx;
	
	bool			is_finish;
	struct s_philo	philo[MAX_PHILO];
	time_t			time_start;
	pthread_t		thread_monitor;
}	t_data;


#endif
