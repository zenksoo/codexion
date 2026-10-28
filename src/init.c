/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/28 03:33:31 by amissa            #+#    #+#             */
/*   Updated: 2026/10/28 22:21:24 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

int	init_coders(t_table *table)
{
	int	i;

	table->coders = malloc(sizeof(t_coder) * table->n);
	if (!table->coders)
		return (1);
	i = -1;
	while (++i < table->n)
	{
		table->coders[i].id = i + 1;
		table->coders[i].table = table;
		// init its mutex
		table->coders[i].compile_count = 0;
		table->coders[i].left = &table->dongles[i];
		table->coders[i].right = &table->dongles[(i + 1) % table->n];
		pthread_mutex_init(&table->coders[i].lock, NULL);
	}
	return (0);
}

int	init_dongles(t_table *table)
{
	int	i;

	table->dongles = malloc(sizeof(t_coder) * table->n);
	if (!table->dongles)
		return (1);
	i = -1;
	while (++i < table->n)
	{
		// init its mutex and cond and init its heap (capacity 2).
		table->dongles[i].busy = 0;
		table->dongles[i].available_at = 0;
		pthread_mutex_init(&table->dongles[i].lock, NULL);
	}
	return (0);
}

int	init_table(t_table *table, char **cli_args)
{
	table->n = atoi(cli_args[0]);
	table->t_burnout = atoi(cli_args[1]);
	table->t_compile = atoi(cli_args[2]);
	table->t_debug = atoi(cli_args[3]);
	table->t_refactor = atoi(cli_args[4]);
	table->compiles_required = atoi(cli_args[5]);
	table->cooldown = atoi(cli_args[6]);
	pthread_mutex_init(&table->log_lock, NULL);
	pthread_mutex_init(&table->state_lock, NULL);
	if (!strcmp(cli_args[7], "fifo"))
		table->use_edf = 0;
	else if (!strcmp(cli_args[7], "edf"))
		table->use_edf = 1;
	if (init_dongles(table))
		return (1);
	if (init_coders(table))
		return (1);
	return (0);
}
