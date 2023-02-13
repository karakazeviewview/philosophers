/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmatsuo <mmatsuo@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 00:21:57 by mmatsuo           #+#    #+#             */
/*   Updated: 2023/02/13 00:42:36 by mmatsuo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

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
	time_t			time_start;
}	t_philo;

typedef struct s_data
{
	time_t			num_philo;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	time_t			philo_must_eat;
	int				dead_num;
	pthread_mutex_t	philo_mtx[MAX_PHILO];
	pthread_mutex_t	fork_mtx[MAX_PHILO];
	pthread_mutex_t	write_finish_mtx;
	bool			is_finish;
	struct s_philo	philo[MAX_PHILO];
	time_t			time_start;
	pthread_t		thread_monitor;
}	t_data;

void		*monitor(void *arg_void);
void		*philo_routine(void *philo_void);
time_t		calc_elapsed_time(time_t *start_time);
time_t		calc_elapsed_time(time_t *start_time);
static bool	is_philo_starve(t_data *arg);
size_t		ft_strlen(const char *s);
time_t		get_time(void);
int			ft_isdigit(int c);
void		ft_putstr_fd(const char *s, int fd);
bool		print_error(const char *str);
static void	check_isnum(char **argv);
static int	ft_check_over(int sign, long ans, char c);
time_t		ft_atol(const char *str);
bool		check_arg(int argc, char **argv);
void		input_data(int argc, char **argv, t_data *data);
void		init_mutex(t_data *data);
void		init_philo(t_data *data, t_philo *philo, int i);
void		init_data(int argc, char **argv, t_data *data);
bool		print_state(t_data *data, time_t now,
				int id, const char *state);
bool		philo_take_fork(t_data *data, t_philo *philo);
bool		philo_eat(t_data *data, t_philo *philo);
bool		philo_sleep(t_data *data, t_philo *philo);
bool		philo_think(t_data *data, t_philo *philo);
static bool	is_philo_starve(t_data *arg);
static bool	is_num_of_eat_reached(t_data *arg);
void		*monitor(void *arg_void);
void		create_thread(t_data *data);
void		join_thread(t_data *data);
void		terminate_data(t_data *data);
int			main(int argc, char **argv);
#endif
