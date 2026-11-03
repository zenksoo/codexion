/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 12:51:59 by amissa            #+#    #+#             */
/*   Updated: 2026/11/03 01:47:38 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <bits/pthreadtypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void	display_data(t_table data)
{
	printf("number of coders: %d\n", data.n);
	printf("time to burnout: %lld\n", data.cli_args->t_burnout);
	printf("time to compile: %lld\n", data.cli_args->t_compile);
	printf("time to debug: %lld\n", data.cli_args->t_debug);
	printf("time to refactor: %lld\n", data.cli_args->t_refactor);
	printf("number of ocmpiles required: %d\n",
		data.cli_args->compiles_required);
	printf("dongle cooldown: %lld\n", data.cli_args->cooldown);
	printf("using edf: %d\n", data.cli_args->scheduler);
}

void	general_cleanup(t_table table)
{
	int	i;

	i = -1;
	while (++i < table.n)
	{
		pthread_mutex_destroy(&table.coders[i]->lock);
		pthread_mutex_destroy(&table.dongles[i]->lock);
		free(table.dongles[i]->queue->nodes);
		free(table.dongles[i]->queue);
		free(table.dongles[i]);
		free(table.coders[i]);
	}
	pthread_mutex_destroy(&table.log_lock);
	pthread_mutex_destroy(&table.state_lock);
	free(table.cli_args);
	free(table.coders);
	free(table.dongles);
}

void display_heap(t_heap *heap)
{
	printf("heap size: %d, and capacity: %d\n", heap->size, heap->cap);

	for (int i = 0; i < heap->size; i++)
		printf("%d - %lld\n", heap->nodes[i].coder_id, heap->nodes[i].key);
}


int	main(int argc, char *argv[])
{
	t_table	table;

	if (cli_argument_parser(argc, argv))
		return (1);
	if (init_table(&table, argv + 1))
		return (general_cleanup(table), 1);

	run_simulation(&table);
	general_cleanup(table);
	return (0);
}
