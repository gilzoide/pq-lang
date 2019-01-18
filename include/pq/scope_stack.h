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

#ifndef __PQ_SCOPE_STACK_H__
#define __PQ_SCOPE_STACK_H__

#include "scope.h"
#include "utils.h"
#include "value.h"

#include <stdlib.h>

/**
 * Dynamic Scope Stack.
 */
typedef struct pq_scope_stack {
	pq_vector_(pq_scope) scopes;
} pq_scope_stack;

/**
 * Initialize Scope Stack with some capacity.
 *
 * @param s                Scope Stack to be initialized.
 * @param initial_capacity Initial Stack array capacity.
 *
 * @return 0 on allocation error, non-zero otherwise.
 */
int pq_scope_stack_initialize(pq_scope_stack *s, size_t initial_capacity);
/**
 * Destroy a Scope Stack, freeing the memory used.
 */
void pq_scope_stack_destroy(pq_context *ctx, pq_scope_stack *s);


/**
 * Get the topmost Scope on Stack.
 */
pq_scope *pq_scope_stack_peek(pq_scope_stack *s);
/**
 * Push a new initialized Scope into a Scope Stack, doubling it's capacity, if
 * necessary.
 *
 * @note The original state of the Scope Stack is maintained in the case of
 *       allocation error, so you still have do `destroy` it anyway.
 *
 * @return Newly allocated scope, or `NULL` on allocation error.
 */
pq_scope *pq_scope_stack_push(pq_scope_stack *s);
/**
 * Pops a Scope from the Stack, decreasing it's size.
 *
 * @return Popped Scope, or `NULL` if there is none.
 */
pq_scope *pq_scope_stack_pop(pq_scope_stack *s);


/**
 * Get a local Value, searching for it from local (topmost) to root Scope.
 */
pq_value *pq_scope_stack_get(const pq_scope_stack *s, pq_symbol sym);
/**
 * Get a Value from the local (topmost) Scope.
 */
pq_value *pq_scope_stack_get_top(const pq_scope_stack *s, pq_symbol sym);
/**
 * Get a Value from the root (bottommost) Scope.
 */
pq_value *pq_scope_stack_get_root(const pq_scope_stack *s, pq_symbol sym);
/**
 * Set a Value in the local (topmost) Scope.
 */
void pq_scope_stack_set(pq_scope_stack *s, pq_symbol sym, pq_value *val);
/**
 * Set a Value in the root (bottommost) Scope.
 */
void pq_scope_stack_set_root(pq_scope_stack *s, pq_symbol sym, pq_value *val);

#endif

