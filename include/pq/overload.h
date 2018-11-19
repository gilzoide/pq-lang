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

/** @file overload.h
 * Pq overload holders, the aggregation of functions with same identifier and
 * different signatures.
 */

#ifndef __PQ_OVERLOAD_H__
#define __PQ_OVERLOAD_H__

#include <Judy.h>

typedef struct pq_context pq_context;
typedef struct pq_type pq_type;
typedef struct pq_value pq_value;

typedef struct pq_overload {
	Pvoid_t function_table;
	Pvoid_t variadic_function_table;
} pq_overload;

/**
 * Creates a new empty Overload.
 */
pq_overload pq_empty_overload(pq_context *ctx);
/**
 * Destroy an Overload.
 */
void pq_overload_destroy(pq_context *ctx, pq_overload *overload);

/**
 * Add a function to Overload.
 *
 * @return Error if `func` is not a function or if signature is already present.
 * @return `func` otherwise.
 */
pq_value *pq_overload_add_function(pq_context *ctx, pq_overload *overload, pq_value *func);
/**
 * Get the overloaded function for the given arguments types.
 *
 * @return Function for the specified signature type.
 * @return `nil` if no overloaded function is found.
 */
pq_value *pq_overload_for_types(pq_context *ctx, pq_overload *overload, size_t n, pq_type **types);

#endif

