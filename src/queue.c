/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/31 18:47:07 by amissa            #+#    #+#             */
/*   Updated: 2026/11/06 22:57:09 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	heap_peek(t_heap *heap, int target)
{
	if (heap->size == 0)
		return (0);
	return (heap->nodes[0].coder_id == target);
}

t_heap_node	heap_pop(t_heap *heap)
{
	t_heap_node	top;

	top.key = 0;
	top.coder_id = -1;
	if (heap->size == 0)
		return (top);
	top = heap->nodes[0];
	heap->size--;
	heap->nodes[0] = heap->nodes[heap->size];
	shift_down(heap, 0);
	return (top);
}

int	heap_push(t_heap *heap, t_coder *coder, int policy)
{
	long long	key;

	if (heap->size == heap->cap)
		return (1);
	if (policy == FIFO)
		key = heap->priority++;
	else
	{
		pthread_mutex_lock(&coder->lock);
		key = coder->last_compile_start + coder->table->args[T_BURNOUT];
		pthread_mutex_unlock(&coder->lock);
	}
	heap->nodes[heap->size].key = key;
	heap->nodes[heap->size].coder_id = coder->id;
	heap->size++;
	shift_up(heap, heap->size - 1);
	return (0);
}

t_heap	*init_queue(int capacity, int scheduler)
{
	t_heap	*heap;

	if (scheduler != FIFO && scheduler != EDF)
		return (NULL);
	heap = malloc(sizeof(t_heap));
	if (!heap)
		return (NULL);
	heap->nodes = malloc(sizeof(t_heap_node) * capacity);
	if (!heap->nodes)
		return (free(heap), NULL);
	heap->size = 0;
	heap->cap = capacity;
	heap->priority = 0;
	return (heap);
}
