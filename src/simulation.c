/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/28 04:45:49 by amissa            #+#    #+#             */
/*   Updated: 2026/10/28 22:50:18 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

void	*coder_routine(t_coder*coder)
{
	for (int i = 0; i < 10000; i++)
		coder->table->val++;
	printf("\033[31m%lld %d has taken a dongle\033[39m\n",
		now_ms() - coder->table->start_time, coder->id);
	printf("\033[31m%lld %d has taken a dongle\033[39m\n",
			now_ms() - coder->table->start_time, coder->id);

	pthread_mutex_lock(&coder->lock);
	coder->last_compile_start = now_ms();
	pthread_mutex_unlock(&coder->lock);


	printf("\033[32m%lld %d is compiling\033[39m\n",
		now_ms() - coder->table->start_time, coder->id);
	usleep(coder->table->t_compile);

	printf("\033[33m%lld %d is debugging\033[39m\n",
		now_ms() - coder->table->start_time, coder->id);
	usleep(coder->table->t_debug);

	printf("\033[35m%lld %d is refactoring\033[39m\n",
		now_ms() - coder->table->start_time, coder->id);
	usleep(coder->table->t_refactor);
	return (NULL);
}

void	*monitor_routine(t_table *table)
{
	printf("monitor checking :) %d\n", table->n);
	return (NULL);
}

void	run_simulation(t_table *table)
{
	int	i;

	table->start_time = now_ms();
	table->val = 0;
	i = -1;
	while (++i < table->n)
	{
		table->coders[i].last_compile_start = table->start_time;
		pthread_create(&table->coders[i].thread,
			NULL,
			(void *(*)(void *))coder_routine,
			&table->coders[i]);
		pthread_join(table->coders[i].thread, NULL);
	}
	// i = -1;
	// while (++i < table->n)
	// 	pthread_join(table->coders[i].thread, NULL);

	printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>> %d\n", table->val);
	pthread_create(&table->monitor,
		NULL,
		(void *(*)(void *))monitor_routine,
		table);
	pthread_join(table->monitor, NULL);

	pthread_join(table->coders[0].thread, NULL);
}
