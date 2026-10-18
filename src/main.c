/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 12:51:59 by amissa            #+#    #+#             */
/*   Updated: 2026/10/18 13:49:01 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


void	*t_func(void *p)
{
	while (1)
	{
		printf("i'm the first thread created %d\n", *(int *)p);
		sleep(1);
	}
	return (NULL);
}



int	main(int argc, char *argv[])
{
	t_args	*data;
	void	*ptr;
	int		n = 200;
	int		n2 = 300;
	pthread_t	id;
	pthread_t	second_id;

	data = malloc(sizeof(t_args));
	if (argc == 9)
	{
		void **args;
		
	
		args = {&data->number_of_coders,
			&data->time_to_burnout, &data->time_to_compile,
			&data->time_to_debug, &data->time_to_refactor,
			&data->number_of_compiles_required,
			&data->dongle_cooldown, &data->scheduler};

		for (int i = 1; i < argc - 1; i++)
		{
			*(int *)args[i - 1] = atoi(argv[i]);
			if (*(int *)args[i - 1] < 0)
				_display_error(INVALID_ARGUMENT, "value must be positive number");
		}
		printf("%s\n", argv[argc - 1]);
		if (!strcmp(argv[argc - 1], "fifo"))
			printf("is fifo hhh\n");
		else if (!strcmp(argv[argc - 1], "edf"))
			printf("is edf hhh\n");
		else
			_display_error(INVALID_ARGUMENT, "invalid value");
	}
	else if (argc > 9)
	{
		_display_error(EXTRA_ARGUMENT, NULL);
		return (1);
	}
	else
	{
		_display_error(MISSING_ARGUMENT, NULL);
		return (1);
	}

	pthread_create(&id, NULL, (void *(*)(void *))t_func, (void *)&n);
	pthread_create(&second_id, NULL, (void *(*)(void *))t_func, (void *)&n2);
	pthread_join(second_id, &ptr);
	return (0);
}
