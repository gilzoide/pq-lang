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

/** @file type_manager.h
 * The Type manager, responsible for interning Type combinations.
 */

#ifndef __PQ_TYPE_MANAGER_H__
#define __PQ_TYPE_MANAGER_H__

#include "type.h"
#include "utils.h"

#include <Judy.h>

/**
 * Type Manager, responsible for interning Type combinations and ownership.
 *
 * Tuples and function signatures will be interned in pq, to ease function
 * overload search.
 */
typedef struct pq_type_manager {
	pq_type *_type;  ///< The Type type.
	pq_type *_error;  ///< The Error type.
	pq_type *_list;  ///< The List type.
	pq_type *_scope;  ///< The Scope type.
	pq_type *_nil;   ///< The Nil type.
	pq_type *_symbol;  ///< The Symbol type.

	pq_type *_bool;  ///< The bool type.
	pq_type *_i8;  ///< The i8 type.
	pq_type *_i16;  ///< The i16 type.
	pq_type *_i32;  ///< The i32 type.
	pq_type *_i64;  ///< The i64 type.
	pq_type *_u8;  ///< The u8 type.
	pq_type *_u16;  ///< The u16 type.
	pq_type *_u32;  ///< The u32 type.
	pq_type *_u64;  ///< The u64 type.

	pq_type *_sys_char;  ///< The system char type.
	pq_type *_sys_short;  ///< The system short type.
	pq_type *_sys_int;  ///< The system int type.
	pq_type *_sys_long;  ///< The system long type.
	pq_type *_sys_uchar;  ///< The system unsigned char type.
	pq_type *_sys_ushort;  ///< The system unsigned short type.
	pq_type *_sys_uint;  ///< The system unsigned int type.
	pq_type *_sys_ulong;  ///< The system unsigned long type.

	pq_type *_float;  ///< The float type.
	pq_type *_double;  ///< The double type.

	pq_type *_string;  ///< The String type.

	pq_type *_pointer;  ///< The generic pointer type.

	pq_type *_function;  ///< The function type.
	pq_type *_c_function;  ///< The C function type.
	pq_type *_native_function;  ///< The Native function type.
	pq_type *_overload;  ///< The overloaded type.

	pq_vector all_types;

	/// Type list -> Tuple type table.
	Pvoid_t tuple_table;
	/// Argument types + return type + is_variadic -> Function signature type table.
	Pvoid_t signature_table;
} pq_type_manager;

/**
 * Initialize a Type Manager.
 */
int pq_type_manager_initialize(pq_type_manager *type_manager);
/**
 * Destroy a Type Manager, freeing the memory used.
 */
void pq_type_manager_destroy(pq_type_manager *type_manager);

/**
 * Enumeration of builtin types.
 */
enum pq_builtin_type {
	PQ_TYPE_TYPE = 0,
	PQ_TYPE_ERROR,
	PQ_TYPE_LIST,
	PQ_TYPE_SCOPE,
	PQ_TYPE_NIL,
	PQ_TYPE_SYMBOL,
	PQ_TYPE_BOOL,
	PQ_TYPE_I8,
	PQ_TYPE_I16,
	PQ_TYPE_I32,
	PQ_TYPE_I64,
	PQ_TYPE_U8,
	PQ_TYPE_U16,
	PQ_TYPE_U32,
	PQ_TYPE_U64,
	PQ_TYPE_SYS_CHAR,
	PQ_TYPE_SYS_SHORT,
	PQ_TYPE_SYS_INT,
	PQ_TYPE_SYS_LONG,
	PQ_TYPE_SYS_UNSIGNED_CHAR,
	PQ_TYPE_SYS_UNSIGNED_SHORT,
	PQ_TYPE_SYS_UNSIGNED_INT,
	PQ_TYPE_SYS_UNSIGNED_LONG,
	PQ_TYPE_FLOAT,
	PQ_TYPE_DOUBLE,
	PQ_TYPE_STRING,
	PQ_TYPE_POINTER,
	PQ_TYPE_FUNCTION,
	PQ_TYPE_C_FUNCTION,
	PQ_TYPE_NATIVE_FUNCTION,
	PQ_TYPE_OVERLOAD,

	PQ_BUILTIN_TYPE_MAX,
};

/**
 * Get a builtin type.
 */
pq_type *pq_get_builtin_type(pq_context *ctx, enum pq_builtin_type builtin_type);

/**
 * Register a new Type in the Context.
 */
pq_type *pq_register_type(pq_context *ctx, const char *name, enum pq_type_kind kind,
                          jit_type_t jit_type, pq_destructor value_destructor);

/**
 * Get the tuple type that contains the given `n` types in order.
 *
 * This returns the same pointer for the same input types.
 */
pq_type *pq_get_tuple_type(pq_context *ctx, size_t n, pq_type **types);

/**
 * Get the signature type that contains the given `n` types in order.
 *
 * This returns the same pointer for the same combination of return and
 * argument types and variadic.
 */
pq_type *pq_get_signature_type(pq_context *ctx, pq_type *return_type,
                               size_t n, pq_type **argument_types, uint8_t is_variadic);

#endif

