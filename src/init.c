/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/28 03:33:31 by amissa            #+#    #+#             */
/*   Updated: 2026/11/03 01:15:33 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdlib.h>

int	init_coders(t_table *table)
{
	int	i;

	table->coders = malloc(sizeof(void *) * table->n);

	if (!table->coders)
		return (1);
	i = -1;
	while (++i < table->n)
	{
		table->coders[i] = malloc(sizeof(t_coder));
		table->coders[i]->id = i + 1;
		table->coders[i]->table = table;
		table->coders[i]->compile_count = 0;
		table->coders[i]->left = table->dongles[i];
		if (table->n == 1)
			table->coders[i]->right = NULL;
		else
			table->coders[i]->right = table->dongles[(i + 1) % table->n];
		pthread_mutex_init(&table->coders[i]->lock, NULL);
	}
	return (0);
}

int	init_dongles(t_table *table)
{
	int	i;

	table->dongles = malloc(sizeof(t_dongle) * table->n);

	if (!table->dongles)
		return (1);
	i = -1;
	while (++i < table->n)
	{
		table->dongles[i] = malloc(sizeof(t_dongle));
		table->dongles[i]->id = i;
		table->dongles[i]->busy = 0;
		table->dongles[i]->available_at = 0;
		table->dongles[i]->queue = init_queue(table->n, table->cli_args->scheduler);
		if (!table->dongles[i]->queue)
			return (1);
		pthread_mutex_init(&table->dongles[i]->lock, NULL);
		pthread_cond_init(&table->dongles[i]->cond, NULL);
	}
	return (0);
}

int	init_table(t_table *table, char **cli_args)
{
	table->n = atoi(cli_args[0]);
	table->cli_args = malloc(sizeof(t_cliargs));
	table->cli_args->t_burnout = atoi(cli_args[1]);
	table->cli_args->t_compile = atoi(cli_args[2]);
	table->cli_args->t_debug = atoi(cli_args[3]);
	table->cli_args->t_refactor = atoi(cli_args[4]);
	table->cli_args->compiles_required = atoi(cli_args[5]);
	table->cli_args->cooldown = atoi(cli_args[6]);
	table->stop = 0;
	table->ready = 0;
	pthread_mutex_init(&table->log_lock, NULL);
	pthread_mutex_init(&table->state_lock, NULL);
	if (!strcmp(cli_args[7], "fifo"))
		table->cli_args->scheduler = FIFO;
	else if (!strcmp(cli_args[7], "edf"))
		table->cli_args->scheduler = EDF;
	if (init_dongles(table))
		return (1);
	if (init_coders(table))
		return (1);
	return (0);
}
