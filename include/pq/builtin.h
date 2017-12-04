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

/** @file builtin.h
 * Pq builtin Types, Values, Functions and Macros.
 */

#ifndef __PQ_BUILTIN_H__
#define __PQ_BUILTIN_H__

#include "cons.h"
#include "function.h"
#include "type.h"
#include "value.h"

// Forward declarations
typedef struct pq_context pq_context;

/**
 * Builtin stuff: just register'em all.
 */
int pq_register_builtin(pq_context *ctx);

/**
 * Builtin pq Types, important stuff to store directly in variables.
 */
typedef struct pq_builtin_types {
	pq_type *_type;  // The "type" type
	pq_type *_error;  // Error type
	pq_type *_cons_cell;  // Cons cells, including code ones
	pq_type *_scope;  // Scopes
	pq_type *_nil;
	pq_type *_symbol;

	pq_type *_function;
	pq_type *_c_function;

	pq_type *_bool;
	pq_type *_i8;
	pq_type *_i16;
	pq_type *_i32;
	pq_type *_i64;
	pq_type *_i128;
	pq_type *_u8;
	pq_type *_u16;
	pq_type *_u32;
	pq_type *_u64;
	pq_type *_u128;

	pq_type *_string;

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


/**
 * Builtin pq Values, important stuff to store directly in variables.
 */
typedef struct pq_builtin_values {
	pq_value *_nil;
	pq_value *_true;
	pq_value *_false;
} pq_builtin_values;
/**
 * Registers the builtin pq Values in the Context.
 */
int pq_register_builtin_values(pq_context *ctx);


/**
 * Registers the builtin pq Functions in the Context.
 */
int pq_register_builtin_functions(pq_context *ctx);

#endif

