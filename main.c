/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmatsuo <mmatsuo@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 00:11:55 by mmatsuo           #+#    #+#             */
/*   Updated: 2023/02/09 20:06:30 by mmatsuo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
# include <stdlib.h>
void	*monitor(void *arg_void);
inline long	calc_elapsed_time(long *start_time);
static bool	is_philo_starve(t_data *arg);


time_t	get_time(void)
{
	struct timeval tp;

	gettimeofday(&tp, NULL);
	return (tp.tv_sec * 1000 + tp.tv_usec / 1000);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

int	ft_isdigit(int c)
{
	return ('0' <= c && c <= '9');
}

void	ft_putstr_fd(const char *s, int fd)
{
	if (!s)
		return ;
	write(fd, s, ft_strlen(s));
}

bool	print_error(const char *str)
{
	ft_putstr_fd(str, 2);
	//printf("%s\n", str);
	return (false);
}

static void	check_isnum(char const **argv)
{
	int	i;
	int	j;

	i = 1;
	while (argv[i])
	{
		j = 0;
		if (argv[i][0] == '\0')
			print_error(ERROR_ARG_INVALID);
		if (argv[i][j] == '+' || argv[i][j] == '-')
			j++;
		while (argv[i][j])
		{
			if (!ft_isdigit(argv[i][j]))
				print_error(ERROR_ARG_INVALID);
			j++;
		}
		i++;
	}
}

static int	ft_check_over(int sign, long ans, char c)
{
	long	tmp;

	tmp = LONG_MAX / 10;
	if (sign == 1)
		c++;
	if (tmp < ans || (tmp == ans && LONG_MAX % 10 + 1 < c - '0'))
		return (1);
	return (0);
}

bool	check_arg(int argc, char const **argv)
{
	int		sign;
	long	ans;
	char	c;

	if (argc < 5 || 6 < argc)
		return (print_error(ERROR_ARG_INVALID));
	check_isnum(/* char const */ argv);
		/* code */
	ft_check_over(/* int */ sign, /* long */ ans , /* char */ c);
		/* code */
	return (true);
}

long	ft_atol(const char *str)
{
	int		sign;
	long	total;

	sign = 1;
	total = 0;
	while (*str == '\t' || *str == '\n' || *str == '\v'
		|| *str == '\f' || *str == '\r' || *str == ' ')
		str++;
	if (*str == '-')
		sign *= -1;
	if (*str == '-' || *str == '+')
		str++;
	while ('0' <= *str && *str <= '9')
	{
		if (sign == 1 && ft_check_over(sign, total, *str))
			return (LONG_MAX);
		if (sign == -1 && ft_check_over(sign, total, *str))
			return (LONG_MIN);
		total = total * 10 + *str - '0';
		str++;
	}
	total *= sign;
	return (total);
}

void	input_data(int argc, char const **argv, t_data *data)
{
	// atoi -> ft_atol
	data->num_philo = ft_atol(argv[1]);
	data->time_to_die = ft_atol(argv[2]);
	data->time_to_eat = ft_atol(argv[3]);
	data->time_to_sleep = ft_atol(argv[4]);
	if (argc == 5)
		data->philo_must_eat = LONG_MAX;
	else
		data->philo_must_eat = atoi(argv[5]);
}

void	init_mutex(t_data *data)
{
	int	i;

	i = 0;
	while (i < MAX_PHILO)
	{
		pthread_mutex_init(&data->philo_mtx[i], NULL);
		pthread_mutex_init(&data->fork_mtx[i], NULL);
		i++;
	}
	pthread_mutex_init(&data->write_finish_mtx, NULL);
	return ;
}

void	init_philo(t_data *data, t_philo *philo, int i)
{
	philo->id = i + 1;
	philo->num_of_eaten = 0;
	philo->time_last_eat = 0;
	philo->fork_left = &data->fork_mtx[i];
	if (i == data->num_philo - 1)
		philo->fork_right = &data->fork_mtx[0];
	else
		philo->fork_right = &data->fork_mtx[i + 1];
	philo->data = data;
}

void	init_data(int argc, char const **argv, t_data *data)
{
	int		i;

	input_data(argc, argv, data);
	init_mutex(data);
	data->is_finish = false;
	i = 0;
	while (i < data->num_philo)
	{
		init_philo(data, &data->philo[i], i);
		i++;
	}
	data->time_start = get_time();
}

bool	print_state(t_data *data, time_t now, int id, const char *state)
{
	pthread_mutex_lock(&data->write_finish_mtx);
	if (data->is_finish)
	{
		pthread_mutex_unlock(&data->write_finish_mtx);
		return (false);
	}
	printf("%ld %d %s\n", now, id, state);
	pthread_mutex_unlock(&data->write_finish_mtx);
	return (true);
}

bool	philo_take_fork(t_data *data, t_philo *philo)
{
	pthread_mutex_lock(philo->fork_left);
	if (!print_state(data, get_time() - data->time_start, philo->id, STATE_FORK))
	{
		pthread_mutex_unlock(philo->fork_left);
		return (false);
	}
	pthread_mutex_lock(philo->fork_right);
	if (!print_state(data, get_time() - data->time_start, philo->id, STATE_FORK))
	{
		pthread_mutex_unlock(philo->fork_left);
		pthread_mutex_unlock(philo->fork_right);
		return (false);
	}
	return (true);
}

bool	philo_eat(t_data *data, t_philo *philo)
{
	if (!philo_take_fork(data, philo))
		return (false);
	pthread_mutex_lock(&data->philo_mtx[philo->id - 1]);
	philo->time_last_eat = get_time() - data->time_start;
	if (!print_state(data, philo->time_last_eat, philo->id, STATE_EAT))
	{
		pthread_mutex_unlock(&data->philo_mtx[philo->id - 1]);
		pthread_mutex_unlock(philo->fork_left);
		pthread_mutex_unlock(philo->fork_right);
		return (false);
	}
	philo->num_of_eaten += 1;
	pthread_mutex_unlock(&data->philo_mtx[philo->id - 1]);
	usleep(data->time_to_eat * 1000); // 個々の関数を最適化すること!!
	pthread_mutex_unlock(philo->fork_left);
	pthread_mutex_unlock(philo->fork_right);
	return (true);
}

bool	philo_sleep(t_data *data, t_philo *philo)
{
	if (!print_state(data, get_time() - data->time_start, philo->id, STATE_SLEEP))
		return (false);
	usleep(data->time_to_sleep * 1000);
	return (true);
}

bool	philo_think(t_data *data, t_philo *philo)
{
	if (!print_state(data, get_time() - data->time_start, philo->id, STATE_THINK))
		return (false);
	usleep(300);
	return (true);
}

void	*philo_routine(void *philo_void)
{
	t_philo	*philo;

	philo = philo_void;
	if (philo->id % 2 == 0)
	{
		if (!philo_think(philo->data, philo))
			return (NULL);
		usleep(100);
	}
	while (1)
	{
		if (!philo_eat(philo->data, philo))
			break ;
		if (!philo_sleep(philo->data, philo))
			break ;
		if (!philo_think(philo->data, philo))
			break ;
	}
	return (NULL);
}

inline long	calc_elapsed_time(long *start_time)
{
	struct timeval	timeval;

	gettimeofday(&timeval, NULL);
	return (timeval.tv_sec * 1000 + timeval.tv_usec / 1000 - *start_time);
}

static bool	is_philo_starve(t_data *arg)
{
	int	i;
	
	i = 0;
	while (i < arg->num_philo)
	{
		pthread_mutex_lock(&arg->philo_mtx[i]);
		if (arg->time_to_die < calc_elapsed_time(&arg->philo[i].time_last_eat))
		{
			pthread_mutex_unlock(&arg->philo_mtx[i]);
			arg->dead_num = 1;
			return (true);
		}
		pthread_mutex_unlock(&arg->philo_mtx[i]);
		i++;
	}
	pthread_mutex_unlock(&arg->philo_mtx[i]);
	return (false);
}

static bool	is_num_of_eat_reached(t_data *arg)
{
	int	i;

	i = 0;
	while (i < arg->num_philo)
	{
		pthread_mutex_lock(&arg->philo_mtx[i]);
		if (arg->philo_must_eat <= arg->philo[i].num_of_eaten)
		{
			pthread_mutex_unlock(&arg->philo_mtx[i]);
			return (true);
		}
		pthread_mutex_unlock(&arg->philo_mtx[i]);
		i++;
	}
	return (false);
}

void	*monitor(void *arg_void)
{
	t_data	*arg;
	
	arg = arg_void;
	usleep(arg->time_to_die / 2);
	while (1)
	{
		if (is_philo_starve(arg))
		{
			pthread_mutex_lock(&arg->write_finish_mtx);
			printf("%ld %d %s\n",
				calc_elapsed_time(&arg->philo[arg->dead_num].time_start),
				arg->dead_num + 1, STATE_DIED);
			arg->is_finish = true;
			pthread_mutex_unlock(&arg->write_finish_mtx);
			break ;
		}
		if (is_num_of_eat_reached(arg))
		{
			pthread_mutex_lock(&arg->write_finish_mtx);
			arg->is_finish = true;
			pthread_mutex_unlock(&arg->write_finish_mtx);
			break ;
		}
	}
	return (NULL);
}

void	create_thread(t_data *data)
{
	int	i;
	
	i = 0;
	while (i < data->num_philo)
	{
		pthread_create(&data->philo[i].thread, NULL,
			philo_routine, &data->philo[i]);
		i++;
	}
	// pthread_create(MONITOR);
	pthread_create(&data->thread_monitor, NULL, monitor, data);
	return ;
}

void	join_thread(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philo)
	{
		pthread_join(data->philo[i].thread, NULL);
		i++;
	}
	// pthread_join(MONITOR);
	pthread_create(&data->thread_monitor, NULL, monitor, data);
	return ;
}

void	terminate_data(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philo)
	{
		pthread_mutex_destroy(&data->philo_mtx[i]);
		pthread_mutex_destroy(&data->fork_mtx[i]);
		i++;
	}
	pthread_mutex_destroy(&data->write_finish_mtx);
}

int main(int argc, char const *argv[])
{
	t_data	data;

	if (!check_arg(argc, argv))
		return (1);
	init_data(argc, argv, &data);
	create_thread(&data);
	join_thread(&data);
	terminate_data(&data);
	return (0);
}

//atoi などの変更 ok
//モニターを作ってみる
//一人の場合の例外処理
//usleep最適化
//エラー処理 ok
