/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/28 04:45:49 by amissa            #+#    #+#             */
/*   Updated: 2026/10/29 23:07:43 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>


void	protected_log(pthread_mutex_t *mutex, long long ms_time, int id, char *msg)
{
	pthread_mutex_lock(mutex);
	printf("\033[38;5;%dm%lld %d %s\033[39m\n", id, ms_time, id, msg);
	pthread_mutex_unlock(mutex);
}


void	*coder_routine(t_coder *coder)
{
	while (!coder->table->stop)
	{

		pthread_mutex_lock(&coder->left->lock);
		pthread_mutex_lock(&coder->right->lock);

		protected_log(&coder->table->log_lock,
			now_ms() - coder->table->start_time,
			coder->id, "has taken a dongle");

		protected_log(&coder->table->log_lock,
			now_ms() - coder->table->start_time,
			coder->id, "has taken a dongle");

		protected_log(&coder->table->log_lock,
			now_ms() - coder->table->start_time,
			coder->id, "is compiling");

		pthread_mutex_lock(&coder->lock);
		coder->last_compile_start = now_ms();
		pthread_mutex_unlock(&coder->lock);

		usleep(coder->table->t_compile);

		coder->compile_count++;

		pthread_mutex_unlock(&coder->right->lock);
		pthread_mutex_unlock(&coder->left->lock);

		protected_log(&coder->table->log_lock,
			now_ms() - coder->table->start_time,
			coder->id, "is debugging");

		usleep(coder->table->t_debug);

		protected_log(&coder->table->log_lock,
			now_ms() - coder->table->start_time,
			coder->id, "is refactoring");

		usleep(coder->table->t_refactor);
	}
	return (NULL);
}

void	*monitor_routine(t_table *table)
{
	int	coder_idx;
	int	all_reach_required_compilation;


	while (!table->stop) {
		coder_idx = -1;
		all_reach_required_compilation = 1;
		while (++coder_idx < table->n)
		{
			pthread_mutex_lock(&table->coders[coder_idx].lock);
			if (now_ms() - table->coders[coder_idx].last_compile_start >= table->t_burnout)
			{
				usleep(10000);
				table->stop = 1;
				printf("%lld %d burnburned out\n", now_ms() - table->start_time, coder_idx);
				break;
			}
			if (table->coders[coder_idx].compile_count < table->compiles_required)
				all_reach_required_compilation = 0;
			pthread_mutex_unlock(&table->coders[coder_idx].lock);
		}
		if (all_reach_required_compilation)
			table->stop = 1;
	}
	return (NULL);
}

void	run_simulation(t_table *table)
{
	int	i;

	table->start_time = now_ms();
	i = -1;
	pthread_create(&table->monitor,
		NULL,
		(void *(*)(void *))monitor_routine,
		table);
	while (++i < table->n)
	{
		table->coders[i].last_compile_start = table->start_time;
		pthread_create(&table->coders[i].thread,
			NULL,
			(void *(*)(void *))coder_routine,
			&table->coders[i]);
	}
	i = -1;
	while (++i < table->n)
		pthread_join(table->coders[i].thread, NULL);
}
