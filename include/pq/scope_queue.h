/*
 * Copyright 2017, 2018 Gil Barbosa Reis <gilzoide@gmail.com>
 * This file is part of pq-lang.
 * 
 * Pq-lang is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Pq-lang is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with pq-lang.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Any bugs should be reported to <gilzoide@gmail.com>
 */

#ifndef __PQ_SCOPE_QUEUE_H__
#define __PQ_SCOPE_QUEUE_H__

#include "scope.h"
#include "value.h"

#include <stdlib.h>

/**
 * Dynamic Scope Queue.
 */
typedef struct pq_scope_queue {
	pq_scope *scopes;
	size_t size;
	size_t capacity;
} pq_scope_queue;

/**
 * Initialize Scope Queue with some capacity.
 *
 * @param q                Scope Queue to be initialized.
 * @param initial_capacity Initial Queue array capacity.
 *
 * @return 0 on allocation error, non-zero otherwise.
 */
int pq_scope_queue_initialize(pq_scope_queue *q, size_t initial_capacity);
/**
 * Destroy a Scope Queue, freeing the memory used.
 */
void pq_scope_queue_destroy(pq_context *ctx, pq_scope_queue *q);


/**
 * Get the topmost Scope on Queue.
 */
pq_scope *pq_scope_queue_peek(pq_scope_queue *q);
/**
 * Push a new initialized Scope into a Scope Queue, doubling it's capacity, if
 * necessary.
 *
 * @note The original state of the Scope Queue is maintained in the case of
 *       allocation error, so you still have do `destroy` it anyway.
 *
 * @return Newly allocated scope, or `NULL` on allocation error.
 */
pq_scope *pq_scope_queue_push(pq_scope_queue *q);
/**
 * Pops a Scope from the Queue, decreasing it's size.
 *
 * @return Popped Scope, or `NULL` if there is none.
 */
pq_scope *pq_scope_queue_pop(pq_scope_queue *q);


/**
 * Get a local Value, searching for it from local (topmost) to root Scope.
 */
pq_value *pq_scope_queue_get(const pq_scope_queue *q, const char *key);
/**
 * Set a Value in the local (topmost) Scope.
 */
void pq_scope_queue_set(pq_scope_queue *q, const char *key, pq_value *val);

#endif

