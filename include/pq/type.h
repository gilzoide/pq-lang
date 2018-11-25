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

#include <stdlib.h>

/**
 * Kinds of types in pq.
 */
enum pq_type_kind {
	PQ_INT,
	PQ_FLOAT,
	PQ_STRING,

	PQ_NIL,
	PQ_BOOL,
	PQ_LIST,
	PQ_TYPE,
	PQ_SCOPE,
	PQ_SYMBOL,
	PQ_ERROR,

	PQ_STRUCT,
	PQ_TUPLE,
	PQ_ARRAY,
	PQ_POINTER,
	PQ_SIGNATURE,

	PQ_FUNCTION,
	PQ_C_FUNCTION,
	PQ_NATIVE_FUNCTION,
	PQ_OVERLOAD,

	PQ_TYPE_KIND_END,

	// index reference enum values
	PQ_TYPE_FIRST_AGGREGATE = PQ_STRUCT,
	PQ_TYPE_LAST_AGGREGATE = PQ_SIGNATURE,
};

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
	pq_destructor value_destructor;
	int kind;
} pq_type;

/**
 * Pq Aggregate Types: types with subtypes: Tuples, Arrays, Pointers, Structs
 * and Signatures.
 */
typedef struct pq_aggregate_type {
	pq_type type;
	pq_type *main_subtype;
	int num_subtypes;
	pq_type *subtypes[0];
} pq_aggregate_type;

/**
 * Create a Type.
 *
 * This will be primarily used by the pq interpreter itself.
 */
pq_type *pq_create_type(const char *name, enum pq_type_kind kind,
                        jit_type_t jit_type, pq_destructor value_destructor);
/**
 * Create an Aggregate Type.
 *
 * The kind passed should be either one of the following: PQ_STRUCT, PQ_TUPLE,
 * PQ_ARRAY, PQ_POINTER, PQ_SIGNATURE.
 * Signatures use the first subtype as the return type.
 *
 * This will be primarily used by the pq interpreter itself.
 */
pq_type *pq_create_aggregate_type(const char *name, enum pq_type_kind kind, jit_type_t jit_type,
                                  pq_type *main_subtype, unsigned int num_subtypes, pq_type **subtypes);

/**
 * Destroy a Type.
 */
void pq_type_destroy(pq_type *type);

/**
 * Get the expected size in bytes of a Value with the specified Type.
 */
size_t pq_type_get_value_size(pq_type *type);

/**
 * Get the Type associated with a JIT type.
 */
pq_type *pq_type_from_jit(jit_type_t jit_type);

////////////////////////////////////////////////////////////////////////////////
//  Accessing aggregate type information
////////////////////////////////////////////////////////////////////////////////
/**
 * Get the return type of a Signature Type.
 *
 * Returns NULL if type is not a Signature.
 */
pq_type *pq_type_get_return_type(pq_type *signature);
/**
 * Get the number of parameters of a Signature Type.
 *
 * Returns -1 if not a Signature.
 */
int pq_type_get_num_arguments(pq_type *signature);
/**
 * Get the argument types of a Signature Type.
 *
 * Returns NULL if type is not a Signature.
 */
pq_type **pq_type_get_argument_types(pq_type *signature);
/**
 * Get if a Signature Type is variadic.
 *
 * Returns -1 if not a Signature.
 */
int pq_type_get_is_variadic(pq_type *signature);

/**
 * Get the number of fields in a Tuple or Struct type.
 *
 * Returns -1 if not a Tuple or Struct type.
 */
int pq_type_get_num_fields(pq_type *aggregate);
/**
 * Get the field types for a Tuple or Struct type.
 *
 * Returns NULL if not a Tuple or Struct type.
 */
pq_type **pq_type_get_field_types(pq_type *aggregate);

/**
 * Get the underlying type for an Array or Pointer type.
 */
pq_type *pq_type_get_underlying_type(pq_type *ptr);

#endif

