/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 12:51:59 by amissa            #+#    #+#             */
/*   Updated: 2026/11/08 15:57:46 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char *argv[])
{
	t_table	table;

	memset(&table, 0, sizeof(table));
	if (cli_argument_parser(argc, argv)
		|| pick_cli_arguments(&table, argv + 1))
		return (1);
	if (init_table(&table) || run_simulation(&table))
		return (write(2, "Error!\n", 7), general_cleanup(&table), 1);
	return (general_cleanup(&table), 0);
}
