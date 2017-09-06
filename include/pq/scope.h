/*
 * Copyright 2017 Gil Barbosa Reis <gilzoide@gmail.com>
 * This file is part of pq-lang.
 * 
 * Pq-lang is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Pega-texto is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with pq-lang.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Any bugs should be reported to <gilzoide@gmail.com>
 */

#ifndef __PQ_SCOPE_H__
#define __PQ_SCOPE_H__

#include "value.h"

#include <stdlib.h>

/**
 * Pq Scopes: a table-like structure that holds value definitions in the
 * form "String -> Value".
 */
typedef struct pq_scope {
	void *table;

	pq_value **created_values;
	size_t size;
	size_t capacity;
} pq_scope;

// Forward declarations
typedef struct pq_context pq_context;

/**
 * Initialize a pq Scope.
 *
 * @param scope Scope to be inicilized.
 */
int pq_scope_initialize(pq_scope *scope);

/**
 * Destroy a Scope, freeing the memory used.
 *
 * @param scope Scope to be destroyed.
 */
void pq_scope_destroy(pq_context *ctx, pq_scope *scope);

/**
 * Get a Value in a Scope.
 *
 * @note The `nil` Value is still a valid #pq_value instance.
 *
 * @param scope Target Scope.
 * @param key   Lookup key.
 *
 * @return Value related to the key, if there is one, `NULL` otherwise.
 */
pq_value *pq_scope_get(const pq_scope *scope, const char *key);

/**
 * Set a Value in a Scope.
 *
 * @param scope Target Scope.
 * @param key   Value's identifier.
 * @param val   Value to be inserted.
 */
void pq_scope_set(pq_scope *scope, const char *key, pq_value *val);

/**
 * Mark a Value as created in Scope, so it can be destroyed when time comes.
 */
int pq_scope_mark_value_for_destruction(pq_scope *scope, pq_value *val);

#endif


