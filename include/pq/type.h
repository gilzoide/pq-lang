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

/** @file type.h
 * Types, metadata about Values.
 */

#ifndef __PQ_TYPE_H__
#define __PQ_TYPE_H__

#include <jit/jit.h>

/**
 * Kinds of types in pq.
 */
typedef enum pq_type_kind {
	PQ_INT,
	PQ_FLOAT,
	PQ_STRING,
	PQ_STRUCT,
	PQ_ARRAY,

	PQ_NIL,
	PQ_LIST,
	PQ_TYPE,
	PQ_SCOPE,
	PQ_SYMBOL,
	PQ_ERROR,

	PQ_FUNCTION,
	PQ_MACRO,
	PQ_C_FUNCTION,
	PQ_C_MACRO,

	PQ_TYPE_KIND_END,
} pq_type_kind;

// Forward declarations
typedef struct pq_context pq_context;
typedef struct pq_value pq_value;

/**
 * Function that destroys a Type or a Value of a specific Type.
 *
 * Parameters:
 * + Pq context.
 * + Data: destroy it's contents wisely, but not itself, as this data is
 *   in the same block of memory as the Value itself.
 */
typedef void (*pq_destructor)(pq_context *, void *);

/**
 * Pq Type: metadata about a Value's type.
 */
typedef struct pq_type {
	char *name;
	jit_type_t jit_type;
	pq_type_kind kind;
	pq_destructor value_destructor;
} pq_type;

/**
 * Register a Type in Context.
 *
 * This will be primarily used by the pq interpreter itself.
 */
pq_value *pq_register_type(pq_context *ctx, const char *name, pq_type_kind kind,
                           jit_type_t jit_type, pq_destructor value_destructor);

/**
 * Destroy a Type.
 */
void pq_type_destroy(pq_context *ctx, pq_type *type);

#endif

