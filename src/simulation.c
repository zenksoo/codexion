/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/28 04:45:49 by amissa            #+#    #+#             */
/*   Updated: 2026/10/28 09:12:51 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <unistd.h>

void	*coder_routine(t_coder*coder)
{
	printf("\033[31m%lld %d has taken a dongle\033[39m\n",
		now_ms() - coder->table->start_time, coder->id);
	usleep(1000);
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
	i = -1;
	while (++i < table->n)
	{
		table->coders[i].last_compile_start = table->start_time;
		pthread_create(&table->coders[i].thread,
			NULL,
			(void *(*)(void *))coder_routine,
			&table->coders[i]);
	}
	pthread_create(&table->monitor,
		NULL,
		(void *(*)(void *))monitor_routine,
		table);
	i = -1;
	while (++i < table->n)
		pthread_join(table->coders[i].thread, NULL);
	pthread_join(table->monitor, NULL);
}
