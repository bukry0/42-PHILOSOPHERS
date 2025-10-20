/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <buket.cili@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 20:53:41 by bcili             #+#    #+#             */
/*   Updated: 2025/10/20 22:55:34 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long    get_value_with_mutex(t_philo *p, int index)
{
    long    value;

    value = 0;
    if (index == 0)
    {
        pthread_mutex_lock(&p->data->dead_mutex);
        value = (long)p->data->dead;
        pthread_mutex_unlock(&p->data->dead_mutex);
    }
    else if (index == 1)
    {
        pthread_mutex_lock(&p->data->eat_mutex);
        value = (long)p->eat_count;
        pthread_mutex_unlock(&p->data->eat_mutex);
    }
    else if (index == 2)
    {
        pthread_mutex_lock(&p->data->meal_mutex);
        value = p->last_meal;
        pthread_mutex_unlock(&p->data->meal_mutex);
    }
    return (value);
}

void set_value_with_mutex(t_philo *p, int index, long new_value)
{
    if (index == 0)
    {
        pthread_mutex_lock(&p->data->dead_mutex);
        p->data->dead = (int)new_value;
        pthread_mutex_unlock(&p->data->dead_mutex);
    }
    else if (index == 1)
    {
        pthread_mutex_lock(&p->data->eat_mutex);
        p->eat_count = (int)new_value;
        pthread_mutex_unlock(&p->data->eat_mutex);
    }
    else if (index == 2)
    {
        pthread_mutex_lock(&p->data->meal_mutex);
        p->last_meal = new_value;
        pthread_mutex_unlock(&p->data->meal_mutex);
    }
}
