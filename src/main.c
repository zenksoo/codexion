/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 12:51:59 by amissa            #+#    #+#             */
/*   Updated: 2026/10/31 14:48:17 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	display_data(t_table data)
{
	printf("number of coders: %d\n", data.n);
	printf("time to burnout: %lld\n", data.t_burnout);
	printf("time to compile: %lld\n", data.t_compile);
	printf("time to debug: %lld\n", data.t_debug);
	printf("time to refactor: %lld\n", data.t_refactor);
	printf("number of ocmpiles required: %d\n",
		data.compiles_required);
	printf("dongle cooldown: %lld\n", data.cooldown);
	printf("using edf: %d\n", data.use_edf);
}

void	general_cleanup(t_table table)
{
	int	i;

	i = -1;
	while (++i < table.n)
	{
		pthread_mutex_destroy(&table.coders[i].lock);
		pthread_mutex_destroy(&table.dongles[i].lock);
	}
	pthread_mutex_destroy(&table.log_lock);
	pthread_mutex_destroy(&table.state_lock);
	free(table.coders);
	free(table.dongles);
}


int	main(int argc, char *argv[])
{
	t_table	table;

	if (cli_argument_parser(argc, argv))
		return (1);
	if (init_table(&table, argv + 1))
		return (1);
	run_simulation(&table);
	general_cleanup(table);
	display_data(table);
	return (0);
}
