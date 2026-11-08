/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cli_argparser.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 23:57:35 by amissa            #+#    #+#             */
/*   Updated: 2026/11/08 17:17:51 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdlib.h>
#include <unistd.h>

int	is_valid_number(char *val)
{
	if (*val == '-' || *val == '+')
		val++;
	while (*val >= '0' && *val <= '9')
		val++;
	if (*val)
		return (0);
	return (1);
}

int	valid_passed_argument(char **argv, int argSize)
{
	int	i;

	i = -1;
	while (++i < argSize)
	{
		if (i == argSize - 1)
		{
			if (strcmp(argv[i], "fifo") && strcmp(argv[i], "edf"))
				return (2);
		}
		else
		{
			if (!*argv[i] || !is_valid_number(argv[i])
				|| atoi(argv[i]) < 0)
				return (1);
		}
	}
	if (atoi(argv[0]) == 0)
		return (1);
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
	return (0);
}

int	cli_argument_parser(int argc, char **argv)
{
	if (argc != 9 || valid_passed_argument(argv + 1, argc - 1))
		return (write(2, "ERROR!\n", 7), 1);
	return (0);
}
