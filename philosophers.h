/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmatsuo <mmatsuo@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 00:12:51 by mmatsuo           #+#    #+#             */
/*   Updated: 2023/02/09 14:54:12 by mmatsuo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <sys/time.h>
# include <unistd.h>

# define MAX_PHILO 200

typedef struct s_philo
{
	int				id;
	int				num_of_eaten;
	time_t			time_last_eat;
	pthread_mutex_t	*fork_left;
	pthread_mutex_t	*fork_right;
	struct s_data	*data;
	ptrhread_t		thread;
}	t_philo;

typedef struct s_data
{
	long			num_philo;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			philo_must_eat;

	pthread_mutex_t	philo_mtx[MAX_PHILO];
	pthread_mutex_t	fork_mtx[MAX_PHILO];
	pthread_mutex_t	write_finish_mtx;
	
	bool			is_finish;
	struct s_philo	philo[MAX_PHILO];
	time_t			time_start;
}	t_data;


#endif
