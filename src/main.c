/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 12:51:59 by amissa            #+#    #+#             */
/*   Updated: 2026/11/03 20:21:58 by amissa           ###   ########.fr       */
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
	printf("time to burnout: %lld\n", data.args[T_BURNOUT]);
	printf("time to compile: %lld\n", data.args[T_COMPILE]);
	printf("time to debug: %lld\n", data.args[T_DEBUG]);
	printf("time to refactor: %lld\n", data.args[T_REFACTOR]);
	printf("number of ocmpiles required: %lld\n",
		data.args[C_REQUIRED]);
	printf("dongle cooldown: %lld\n", data.args[COOLDOWN]);
	printf("using edf: %lld\n", data.args[POLICY]);
}

void	general_cleanup(t_table table)
{
	int	i;

	i = -1;
	while (++i < table.n)
	{
		if(table.coders[i]->initialized_mutex)
			pthread_mutex_destroy(&table.coders[i]->lock);
		free(table.coders[i]->last_compile_start);
		free(table.coders[i]);
		if (table.dongles[i]->initialized_mutex)
			pthread_mutex_destroy(&table.dongles[i]->lock);
		free(table.dongles[i]->queue->nodes);
		free(table.dongles[i]->queue);
		free(table.dongles[i]);
	}
	if (table.initialized_mutex[0])
		pthread_mutex_destroy(&table.log_lock);
	if (table.initialized_mutex[1])
		pthread_mutex_destroy(&table.state_lock);
	free(table.args);
	free(table.coders);
	free(table.dongles);
	free(table.start_time);
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
	if (pick_cli_arguments(&table, argv + 1) || init_table(&table))
		return (general_cleanup(table), 1);
	if (run_simulation(&table))
		return (general_cleanup(table), 1);
	general_cleanup(table);
	return (0);
}
