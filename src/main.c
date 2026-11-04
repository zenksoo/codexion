/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 12:51:59 by amissa            #+#    #+#             */
/*   Updated: 2026/11/04 13:21:40 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdio.h>
#include <string.h>

int	main(int argc, char *argv[])
{
	t_table	table;

	memset(&table, 0, sizeof(table));

	if (cli_argument_parser(argc, argv))
		return (1);
	if (pick_cli_arguments(&table, argv + 1) || init_table(&table))
	{
		fprintf(stderr, "Error!\n");
		return (general_cleanup(table), 1);
	}
	if (run_simulation(&table))
	{
		fprintf(stderr, "Error!\n");
		return (general_cleanup(table), 1);
	}
	return (general_cleanup(table), 0);
}
