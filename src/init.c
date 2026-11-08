/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/28 03:33:31 by amissa            #+#    #+#             */
/*   Updated: 2026/11/08 17:17:45 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <string.h>

int	init_condtion_variable(t_dongle *dongle, int clock)
{
	if (pthread_condattr_init(&dongle->attr))
		return (1);
	dongle->destroy_conds[1] = 1;
	if (pthread_condattr_setclock(&dongle->attr, clock))
		return (1);
	if (pthread_cond_init(&dongle->cond, &dongle->attr))
		return (1);
	dongle->destroy_conds[0] = 1;
	return (0);
}

int	init_coders(t_table *table)
{
	int	i;

	table->coders = malloc(sizeof(void *) * table->n);
	if (!table->coders)
		return (1);
	memset(table->coders, 0, sizeof(void *) * table->n);
	i = -1;
	while (++i < table->n)
	{
		table->coders[i] = malloc(sizeof(t_coder));
		if (!table->coders[i])
			return (1);
		table->coders[i]->id = i + 1;
		table->coders[i]->table = table;
		table->coders[i]->compile_count = 0;
		table->coders[i]->left = table->dongles[i];
		table->coders[i]->right = table->dongles[(i + 1) % table->n];
		table->coders[i]->initialized_mutex = 0;
		if (pthread_mutex_init(&table->coders[i]->lock, NULL))
			return (1);
		table->coders[i]->initialized_mutex = 1;
	}
	return (0);
}

int	init_dongles(t_table *table)
{
	int	i;

	table->dongles = malloc(sizeof(void *) * table->n);
	if (!table->dongles)
		return (1);
	memset(table->dongles, 0, sizeof(void *) * table->n);
	i = -1;
	while (++i < table->n)
	{
		table->dongles[i] = malloc(sizeof(t_dongle));
		if (!table->dongles[i])
			return (1);
		table->dongles[i]->id = i;
		table->dongles[i]->busy = 0;
		table->dongles[i]->available_at = 0;
		table->dongles[i]->destroy_mutex = 0;
		memset(table->dongles[i]->destroy_conds, 0, 8);
		table->dongles[i]->queue = init_queue(table->n, table->args[POLICY]);
		if (!table->dongles[i]->queue
			|| pthread_mutex_init(&table->dongles[i]->lock, NULL))
			return (1);
		table->dongles[i]->destroy_mutex = 1;
		if (init_condtion_variable(table->dongles[i], CLOCK_MONOTONIC))
			return (1);
	}
	return (0);
}


int	init_table(t_table *table)
{
	table->stop = 0;
	table->ready = 0;
	memset(table->destroy_mutex, 0, 8);
	if (init_dongles(table) || init_coders(table))
		return (1);
	if (pthread_mutex_init(&table->log_lock, NULL))
		return (1);
	table->destroy_mutex[0] = 1;
	if (pthread_mutex_init(&table->state_lock, NULL))
		return (1);
	table->destroy_mutex[1] = 1;
	return (0);
}
