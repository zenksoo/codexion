/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 12:51:59 by amissa            #+#    #+#             */
/*   Updated: 2026/10/26 00:14:21 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdbool.h>

void	display_data(t_args *data)
{
	printf("number of coders: %d\n", data->number_of_coders);
	printf("time to burnout: %d\n", data->time_to_burnout);
	printf("time to compile: %d\n", data->time_to_compile);
	printf("time to debug: %d\n", data->time_to_debug);
	printf("tim to refactor: %d\n", data->time_to_refactor);
	printf("number of ocmpiles required: %d\n",
		data->number_of_compiles_required);
	printf("dongle cooldown: %d\n", data->dongle_cooldown);
	printf("scheduler: %s\n", data->scheduler);
}

int	main(int argc, char *argv[])
{
	t_args	*data;
	int		cli_parser_status;

	data = malloc(sizeof(t_args));
	cli_parser_status = cli_argument_parser(argc, argv, data);
	if (cli_parser_status)
		return (cli_parser_status);
	display_data(data);
	return (free(data), 0);
}
