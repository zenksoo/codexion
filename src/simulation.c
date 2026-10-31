/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/28 04:45:49 by amissa            #+#    #+#             */
/*   Updated: 2026/10/31 15:04:42 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>

void	protected_log(pthread_mutex_t *mutex, long long ms_time, int id, char *msg)
{
	pthread_mutex_lock(mutex);
	printf("\033[38;5;%dm%lld %d %s\033[39m\n", id, ms_time, id, msg);
	pthread_mutex_unlock(mutex);
}

void wait_for_start(void *arg)
{
	t_table *table;
    int ready;

	table = (t_table *)arg;
    ready = 0;
    while (!ready)
    {
        pthread_mutex_lock(&table->state_lock);
        ready = table->ready;
        pthread_mutex_unlock(&table->state_lock);
        if (!ready)
            usleep(100);
    }
}

void	stop_simulation(t_table *table)
{
    int i;

    pthread_mutex_lock(&table->state_lock);
	printf("set stop flag\n");
    table->stop = 1;
    pthread_mutex_unlock(&table->state_lock);
    i = table->n;
	while (--i >= 0)
    {
		printf("try to broadcast: %d\n", i);
		pthread_mutex_lock(&table->dongles[i].lock);
		pthread_cond_broadcast(&table->dongles[i].cond);
		printf("dongle: %d broadcasted\n", i);
		pthread_mutex_unlock(&table->dongles[i].lock);
	}
}

void	run_simulation(t_table *table)
{
	int	i;

	i = -1;
	pthread_create(&table->monitor,
		NULL,
		(void *(*)(void *))monitor_routine,
		table);

	while (++i < table->n)
	{
		pthread_mutex_lock(&table->coders[i].lock);
		table->coders[i].last_compile_start = now_ms();
		pthread_mutex_unlock(&table->coders[i].lock);
		pthread_create(&table->coders[i].thread,
			NULL,
			(void *(*)(void *))coder_routine,
			&table->coders[i]);
	}
	table->start_time = now_ms();
	table->ready = 1;
	i = -1;
	while (++i < table->n)
	{
		pthread_join(table->coders[i].thread, NULL);
	}

	pthread_join(table->monitor, NULL);
}
