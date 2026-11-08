/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 12:58:33 by amissa            #+#    #+#             */
/*   Updated: 2026/11/08 17:19:30 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <pthread.h>
# include <stdlib.h>
# include <time.h>

# define FIFO 1
# define EDF 2

# define TOTAL_ARGS 7

# define T_BURNOUT 0
# define T_COMPILE 1
# define T_DEBUG 2
# define T_REFACTOR 3
# define COOLDOWN 4
# define C_REQUIRED 5
# define POLICY 6

typedef struct s_heap_node
{
	long long	key;
	int			coder_id;
}	t_heap_node;

typedef struct s_heap
{
	t_heap_node	*nodes;
	int			size;
	int			cap;
	int			priority;
}	t_heap;

typedef struct s_dongle
{
	long long			available_at;
	pthread_mutex_t		lock;
	pthread_cond_t		cond;
	pthread_condattr_t	attr;
	int					destroy_mutex;
	int					destroy_conds[2]; // [0] for cond, [1] for condattr
	int					id;
	int					busy;
	t_heap				*queue;
}	t_dongle;

typedef struct s_coder
{
	long long		last_compile_start;
	pthread_t		thread;
	t_dongle		*right;
	t_dongle		*left;
	pthread_mutex_t	lock;
	int				initialized_mutex;
	int				id;
	int				compile_count;
	struct s_table	*table;
}	t_coder;

typedef struct s_table
{
	int				n;
	long long		*args;
	long long		start_time;
	int				stop;
	int				ready;
	pthread_mutex_t	state_lock;
	pthread_mutex_t	log_lock;
	int				destroy_mutex[2]; // [0] for log, [1] for state
	t_dongle		**dongles;
	t_coder			**coders;
	pthread_t		monitor;
}	t_table;

// initial all needed resources
int			init_condtion_variable(t_dongle *dongle, int clock);
int			init_table(t_table *table);
int			init_dongles(t_table *table);
int			init_coders(t_table *table);

// shared methods between threads
int			wait_for_start(void *arg);
int			is_stoped(t_table *table);

// threads functions
void		*coder_routine(void *arg);
void		*monitor_routine(void *arg);

int			create_coders_routine(t_table *table);

// free memeory
int			destroy_dongles(t_table *table);
int			destroy_coders(t_table *table);
void		general_cleanup(t_table *table);

// method for parsing argument
int			pick_cli_arguments(t_table *table, char **cli_args);
int			valid_passed_argument(char **argv, int argSize);
int			cli_argument_parser(int argc, char **argv);

// methods for heap implementation
void		swap_nodes(t_heap_node *a, t_heap_node *b);
int			is_better(t_heap_node a, t_heap_node b);
t_heap		*init_queue(int capacity, int scheduler);
t_heap_node	heap_pop(t_heap *heap);
int			heap_push(t_heap *heap, t_coder *coder, int policy);
int			heap_peek(t_heap *heap, int target);
void		shift_up(t_heap *heap, int i);
void		shift_down(t_heap *heap, int i);
void		heap_remove(t_heap *heap, int id);
void		destroy_queue(t_heap *heap);

// coder actions
void		wait_dongle(t_dongle *d);
int			take_dongles(t_coder *coder);
int			take_dongle(t_coder *coder, t_table *table, t_dongle *dongle);
int			release_both_dongles(t_coder *coder);
int			release_dongle(t_dongle *dongle);
int			compile(t_coder *coder);
void		wake_coders(t_table *table);

// entry-point of the simulation
int			run_simulation(t_table *table);
int			simulation_sleep(t_table *table, long long ms);

int			radio_log(t_coder *coder, char *msg);

long long	now_ms(void);
#endif
