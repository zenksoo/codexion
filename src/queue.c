/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amissa <amissa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/31 18:47:07 by amissa            #+#    #+#             */
/*   Updated: 2026/11/01 22:33:12 by amissa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


void heap_push(t_heap *heap, t_coder *new_coder)
{
	t_heap_node *tmp;

	if (heap->size == 2 || !heap || !new_coder)
		return ;

	tmp = realloc(heap->nodes, (sizeof(t_heap_node) * (heap->size + 1)));

	heap->nodes = tmp;

	heap->nodes[heap->size].coder_id = new_coder->id;
	if (new_coder->table->scheduler == FIFO)
		heap->nodes[heap->size].key = now_ms();
	else
		heap->nodes[heap->size].key = new_coder->last_compile_start + new_coder->table->t_burnout;
	heap->size++;
}


int	heap_peek(t_heap *heap, int scheduler)
{
	t_heap_node	node;
	int			peeked_idx;
	int			i;

	if (!heap->size)
		return -1;

	peeked_idx = 0;
	if (scheduler == EDF)
	{
		i = 0;
		while (++i < heap->size)
			if (heap->nodes[i].key < node.key)
				peeked_idx = i;
	}
	node = heap->nodes[peeked_idx];
	i = -1;
	while (++i < heap->size - 1)
	{
		if (i >= peeked_idx)
			heap->nodes[i] = heap->nodes[i + 1];
	}
	heap->size--;
	heap->nodes = realloc(heap->nodes, (sizeof(t_heap_node) * heap->size));
	return node.coder_id;
}

t_heap *init_queue(int scheduler)
{
	t_heap *heap;


	if (scheduler != FIFO && scheduler != EDF)
		return NULL;
	heap = malloc((sizeof(*heap)));
	heap->size = 0;
	return (heap);
}
