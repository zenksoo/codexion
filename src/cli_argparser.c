/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cli_argparser.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 23:57:35 by amissa            #+#    #+#             */
/*   Updated: 2026/10/26 00:12:44 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	pick_arguments(char **args, t_args *data)
{
	data->number_of_coders = atoi(args[0]);
	data->time_to_burnout = atoi(args[1]);
	data->time_to_compile = atoi(args[2]);
	data->time_to_debug = atoi(args[3]);
	data->time_to_refactor = atoi(args[4]);
	data->number_of_compiles_required = atoi(args[5]);
	data->dongle_cooldown = atoi(args[6]);
	if (!strcmp(args[7], "fifo"))
		data->scheduler = "fifo";
	else if (!strcmp(args[7], "edf"))
		data->scheduler = "edf";
}

int	valid_passed_argument(char **argv, int argSize)
{
	int	i;

	i = -1;
	while (++i < argSize)
	{
		if (i == argSize - 1)
		{
			if (strcmp(argv[7], "fifo") && strcmp(argv[7], "edf"))
				return (2);
		}
		else
		{
			if (atoi(argv[i]) < 0)
				return (1);
		}
	}
	return (0);
}

int	cli_argument_parser(int argc, char **argv, t_args *data)
{
	int	validation_status;

	if (argc == 9)
	{
		validation_status = valid_passed_argument(argv + 1, argc - 1);
		if (!validation_status)
			pick_arguments(argv + 1, data);
		else if (validation_status == 1)
			return (free(data), _display_error(
					INVALID_ARGUMENT,
					"the first 7 arguments must be posititive integer"));
		else if (validation_status == 2)
			return (free(data), _display_error(
					INVALID_ARGUMENT,
					"the last argument must be 'fifo' or 'edf'"));
	}
	else if (argc > 9)
		return (free(data), _display_error(EXTRA_ARGUMENT, NULL));
	else
		return (free(data), _display_error(MISSING_ARGUMENT, NULL));
	return (0);
}
