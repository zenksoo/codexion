/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zenkso <zenkso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 12:51:59 by amissa            #+#    #+#             */
/*   Updated: 2026/10/18 13:57:41 by zenkso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


void pick_arguments(char **args, t_args *data) {
    // there is 8 argument
    // the first 7 must be positive integer
    // last one is string with value of "fifo" or "edf"
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

int valid_passed_argument(char **args, int argSize) {
    int i;

    i = -1;
    while (++i < argSize)
    {
        if (i == argSize - 1)
        {
            if (strcmp(args[7], "fifo") && strcmp(args[7], "edf"))
                return 2;
        }
        else
        {
            if (atoi(args[i]) < 0)
                return 1;
        }
    }
    return 0;
}


int	main(int argc, char *argv[])
{
	t_args	*data;

	data = malloc(sizeof(t_args));
	if (argc == 9)
	{
        int validation_status;

        validation_status = valid_passed_argument(argv + 1, argc - 1);

        if (!validation_status)
            pick_arguments(argv + 1, data);
        else if (validation_status == 1)
            return (free(data), _display_error(INVALID_ARGUMENT, "the first 7 arguments must be posititive integer"));
        else if (validation_status == 2)
            return (free(data), _display_error(INVALID_ARGUMENT, "the last argument must be 'fifo' or 'edf'"));
	}
	else if (argc > 9)
		return (free(data), _display_error(EXTRA_ARGUMENT, NULL));
	else
		return (free(data), _display_error(MISSING_ARGUMENT, NULL));
	return (0);
}
