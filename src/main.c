/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 12:51:59 by amissa            #+#    #+#             */
/*   Updated: 2026/10/28 08:29:29 by amissa           ###   ########.fr       */
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

int	main(int argc, char *argv[])
{
	t_table	table;

	if (cli_argument_parser(argc, argv))
		return (1);
	if (init_table(&table, argv + 1))
		return (1);
	run_simulation(&table);
	display_data(table);
	free(table.coders);
	free(table.dongles);
	return (0);
}
