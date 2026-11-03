/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/28 03:33:31 by amissa            #+#    #+#             */
/*   Updated: 2026/11/03 20:26:33 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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

int	pick_cli_arguments(t_table *table, char **cli_args)
{
	table->n = atoi(cli_args[0]);
	table->args = malloc(sizeof(long long) * TOTAL_ARGS);
	if (!table->args)
		return (1);
	table->args[T_BURNOUT] = atoi(cli_args[1]);
	table->args[T_COMPILE] = atoi(cli_args[2]);
	table->args[T_DEBUG] = atoi(cli_args[3]);
	table->args[T_REFACTOR] = atoi(cli_args[4]);
	table->args[C_REQUIRED] = atoi(cli_args[5]);
	table->args[COOLDOWN] = atoi(cli_args[6]);
	if (!strcmp(cli_args[7], "fifo"))
		table->args[POLICY] = FIFO;
	else if (!strcmp(cli_args[7], "edf"))
		table->args[POLICY] = EDF;
	return 0;
}

int	init_table(t_table *table)
{
	table->stop = 0;
	table->ready = 0;

	table->initialized_mutex[0] = 0;
	table->initialized_mutex[1] = 0;

	if (pthread_mutex_init(&table->log_lock, NULL))
		return (1);
	if (pthread_mutex_init(&table->state_lock, NULL))
		return (1);
	table->initialized_mutex[0] = 1;
	table->initialized_mutex[1] = 1;
	if (init_dongles(table))
		return (1);
	if (init_coders(table))
		return (1);
	return (0);
}
