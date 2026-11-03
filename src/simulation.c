/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/28 04:45:49 by amissa            #+#    #+#             */
/*   Updated: 2026/11/04 00:49:09 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>
#include <stdio.h>

int	radio_log(t_coder *coder, char *msg)
{
	t_table	*table;

	table = coder->table;
	if (pthread_mutex_lock(&table->log_lock))
		return (1);
	printf("%lld %d %s\n", now_ms() - table->start_time, coder->id, msg);
	fflush(stdout);
	if (pthread_mutex_unlock(&table->log_lock))
		return (1);
	return (0);
}

int	simulation_sleep(t_table *table, long long ms)
{
	long long	target;

	target = now_ms() + ms;
	while (now_ms() < target && !is_stoped(table))
		usleep(500);
	if (is_stoped(table))
		return (1);
	return (0);
}

int	wait_for_start(void *arg)
{
	t_table	*table;
	int		ready;

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
	return (0);
}

void	stop_simulation(t_table *table)
{
	int	i;

	pthread_mutex_lock(&table->state_lock);
	table->stop = 1;
	pthread_mutex_unlock(&table->state_lock);
	i = -1;
	while (++i < table->n)
	{
		pthread_mutex_lock(&table->dongles[i]->lock);
		pthread_cond_broadcast(&table->dongles[i]->cond);
		pthread_mutex_unlock(&table->dongles[i]->lock);
	}
}


int	create_coders_routine(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->n)
	{
		if (pthread_mutex_lock(&table->coders[i]->lock))
			return (1);
		table->coders[i]->last_compile_start = now_ms();
		if (pthread_mutex_unlock(&table->coders[i]->lock))
			return (1);
		if (pthread_create(&table->coders[i]->thread,
			NULL, (void *(*)(void *))coder_routine,
			table->coders[i]))
			return (1);
	}
	return (0);
}

int	run_simulation(t_table *table)
{
	int	i;

	i = -1;
	// table->start_time = malloc(sizeof(struct timespec));
	table->start_time = now_ms();
	if (pthread_create(&table->monitor, NULL,
		(void *(*)(void *))monitor_routine, table))
		return (1);
	if (create_coders_routine(table))
		return (1);

	if (pthread_mutex_lock(&table->state_lock))
		return (1);
	table->ready = 1;
	table->ready_coder_id = 1;
	if (pthread_mutex_unlock(&table->state_lock))
		return (1);
	i = -1;
	if (pthread_join(table->monitor, NULL))
		return (1);
	while (++i < table->n)
		if (pthread_join(table->coders[i]->thread, NULL))
			return (1);
	return 0;
}
