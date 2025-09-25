/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 12:12:15 by bcili             #+#    #+#             */
/*   Updated: 2025/09/01 20:38:13 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_data
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat_count;
	int				dead;
	long			start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
}	t_data;

typedef struct s_philo
{
	int			id;
	int			eat_count;
	long		last_meal;
	t_data		*data;
	pthread_t	thread;
}	t_philo;

// Utils
int		ft_atoi(const char *str);
long	get_timestamp_ms(void);
int		error_exit(const char *msg);

// Init
t_philo	*init_philos(t_data *data);
void	print_death(t_philo *p, t_data *data, long now);

// Routine
void	*philo_routine(void *arg);
void	take_forks(t_philo *p);
void	put_forks(t_philo *p);
void	print_status(t_philo *p, char *status);

// Monitor
void	*monitor_routine(void *arg);

#endif