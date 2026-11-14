/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zenkso <zenkso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 19:56:16 by amissa            #+#    #+#             */
/*   Updated: 2026/11/12 12:52:00 by zenkso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void wait_dongle(t_dongle *d) {
  struct timespec ts;
  long long wake;

  wake = now_ms() + 5;
  if (!d->busy)
    wake = d->available_at;
  ts.tv_sec = wake / 1000;
  ts.tv_nsec = (wake % 1000) * 1000000;
  pthread_cond_timedwait(&d->cond, &d->lock, &ts);
}

void updated_dongle_cooldown(t_table *table, t_dongle *dongle) {
  pthread_mutex_lock(&dongle->lock);
  dongle->available_at = now_ms() + table->args[COOLDOWN];
  pthread_mutex_unlock(&dongle->lock);
}

int release_dongle(t_dongle *dongle) {
  pthread_mutex_lock(&dongle->lock);
  dongle->busy = 0;
  pthread_cond_broadcast(&dongle->cond);
  pthread_mutex_unlock(&dongle->lock);
  return (0);
}

int release_both_dongles(t_coder *coder) {
  if (release_dongle(coder->left))
    return (1);
  updated_dongle_cooldown(coder->table, coder->left);
  if (release_dongle(coder->right))
    return (1);
  updated_dongle_cooldown(coder->table, coder->right);
  return (0);
}

int take_dongle(t_coder *coder, t_table *table, t_dongle *dongle) {
  pthread_mutex_lock(&dongle->lock);
  if (heap_push(dongle->queue, coder, table->args[POLICY]))
    return (pthread_mutex_unlock(&dongle->lock), 1);
  while (!is_stoped(table) &&
         (dongle->busy || now_ms() < dongle->available_at ||
          !heap_peek(dongle->queue, coder->id)))
    wait_dongle(dongle);
  if (is_stoped(table))
    return (pthread_mutex_unlock(&dongle->lock), 1);
  heap_pop(dongle->queue);
  dongle->busy = 1;
  pthread_mutex_unlock(&dongle->lock);
  return (0);
}

int take_dongles(t_coder *coder) {
  // struct timespec ts; // time to wait until dongle be avaible

  pthread_mutex_lock(&coder->left->lock);
  heap_push(coder->left->queue, coder, coder->table->args[POLICY]);
  pthread_mutex_unlock(&coder->left->lock);
  pthread_mutex_lock(&coder->right->lock);
  heap_push(coder->right->queue, coder, coder->table->args[POLICY]);
  pthread_mutex_unlock(&coder->right->lock);

  // check i'm the first in the queue on both dongles
  //
  while (!heap_peek(coder->left->queue, coder->id))
    ;
  while (!heap_peek(coder->right->queue, coder->id))
    ;
  while (coder->left->busy && coder->right->busy)
    ;

  while (coder->left->available_at > coder->right->available_at &&
         now_ms() < coder->left->available_at)
    wait_dongle(coder->left);
  while (coder->right->available_at > coder->left->available_at &&
         now_ms() < coder->right->available_at)
    wait_dongle(coder->right);

  pthread_mutex_lock(&coder->right->lock);
  heap_pop(coder->right->queue);
  coder->right->busy = 1;
  pthread_mutex_unlock(&coder->right->lock);
  pthread_mutex_lock(&coder->left->lock);
  heap_pop(coder->left->queue);
  coder->left->busy = 1;
  pthread_mutex_unlock(&coder->left->lock);
  return (0);
}
