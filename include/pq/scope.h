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

/**
 * Pq Scopes: a table-like structure that holds value definitions in the
 * form "String -> Value".
 */
typedef void *pq_scope;

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
void pq_scope_destroy(pq_scope *scope);

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
 *
 * @return Previous Value identified by `key` (may be `NULL`).
 */
pq_value *pq_scope_set(pq_scope *scope, const char *key, pq_value *val);

#endif


