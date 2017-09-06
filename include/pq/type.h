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

/** @file type.h
 * Types, metadata about Values.
 */

#ifndef __PQ_TYPE_H__
#define __PQ_TYPE_H__

/**
 * Kinds of types in pq.
 */
typedef enum pq_type_kind {
	PQ_NIL,
	PQ_LLVM,
	PQ_CONS_CELL,
	PQ_TYPE,
	PQ_SCOPE,
	PQ_SYMBOL,
	PQ_ERROR,
	PQ_FUNCTION,
	PQ_MACRO,
	PQ_C_FUNCTION,
	PQ_LLVM_MACRO,

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
 * + Data: destroy it wisely.
 */
typedef void (*pq_destructor)(pq_context *, void *);

/**
 * Pq Type: metadata about a Value's type.
 */
typedef struct pq_type_t {
	char *name;
	pq_type_kind kind;
	struct pq_type_t *extends;
	pq_destructor self_destructor;
	pq_destructor value_destructor;
	void *data;
} pq_type;


/**
 * Register a Type in Context.
 *
 * This will be primarily used by the pq interpreter itself.
 */
pq_value *pq_register_type(pq_context *ctx, const char *name, pq_type_kind kind,
                           pq_type *extends, pq_destructor self_destructor,
                           pq_destructor value_destructor, void *data);

/**
 * Builtin pq types, important stuff to store directly in variables.
 */
typedef struct {
	pq_type *_type;  // The "type" type
	pq_type *_error;  // Error type
	pq_type *_cons_cell;  // Cons cells, including code ones
	pq_type *_scope;  // Scopes

	pq_type *_i1;
	pq_type *_i8;
	pq_type *_i16;
	pq_type *_i32;
	pq_type *_i64;

	pq_type *_half;
	pq_type *_float;
	pq_type *_double;

	pq_type *_pointer;
	pq_type *_struct;
	pq_type *_array;
} pq_builtin_types;

/**
 * Registers the builtin pq Types in the Context.
 */
int pq_register_builtin_types(pq_context *ctx);

#endif

