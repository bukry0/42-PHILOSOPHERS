/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 18:06:49 by bcili             #+#    #+#             */
/*   Updated: 2025/10/24 18:07:05 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_forks(t_philo *p)
{
	int	left;
	int	right;

	left = p->id - 1;
	right = p->id % p->data->num_philos;
	if (p->id % 2 == 0)
	{
		pthread_mutex_lock(&p->data->forks[right]);
		print_status(p, "has taken a fork");
		pthread_mutex_lock(&p->data->forks[left]);
		print_status(p, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&p->data->forks[left]);
		print_status(p, "has taken a fork");
		pthread_mutex_lock(&p->data->forks[right]);
		print_status(p, "has taken a fork");
	}
}

void	put_forks(t_philo *p)
{
	pthread_mutex_unlock(&p->data->forks[p->id - 1]);
	pthread_mutex_unlock(&p->data->forks[p->id % p->data->num_philos]);
}

int	eating(t_philo *p)
{
	take_forks(p);
	print_status(p, "is eating");
	set_value_with_mutex(p, 2, get_timestamp_ms());
	ft_usleep(p->data->time_to_eat);
	pthread_mutex_lock(&p->data->eat_mutex);
	p->eat_count++;
	if (p->data->must_eat_count != -1
		&& p->eat_count >= p->data->must_eat_count)
	{
		pthread_mutex_unlock(&p->data->eat_mutex);
		put_forks(p);
		return (0);
	}
	pthread_mutex_unlock(&p->data->eat_mutex);
	put_forks(p);
	return (1);
}

static void	*one_philo_routine(void *arg)
{
	t_philo	*p;

	p = (t_philo *)arg;
	print_status(p, "is thinking");
	usleep((p->data->time_to_die) * 1000);
	print_status(p, "died");
	set_value_with_mutex(p, 0, 1);
	return (NULL);
}

void	*philo_routine(void *arg)
{
	t_philo	*p;

	p = (t_philo *)arg;
	if (p->data->num_philos == 1)
		return (one_philo_routine(arg));
	if (p->id % 2 != 0)
		usleep(1000);
	while (!(int)get_value_with_mutex(p, 0) && (p->data->must_eat_count == -1
			|| (int)get_value_with_mutex(p, 1) < p->data->must_eat_count))
	{
		print_status(p, "is thinking");
		if (!eating(p))
			return (NULL);
		print_status(p, "is sleeping");
		ft_usleep(p->data->time_to_sleep);
	}
	return (NULL);
}
