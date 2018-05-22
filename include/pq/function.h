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

/** @file function.h
 * Pq callable functions, defined either through pq or C.
 */

#ifndef __PQ_FUNCTION_H__
#define __PQ_FUNCTION_H__

// Forward declarations
typedef struct pq_context pq_context;
typedef struct pq_type pq_type;
typedef struct pq_value pq_value;

#include "list.h"

#include <jit/jit-function.h>

#include <stdint.h>

/**
 * Flags for Functions, to be ORed and stored in the metadata.
 */
enum pq_function_flags {
	PQ_VARIADIC       = (1 << 0),
	PQ_EVAL_ARGS      = (1 << 1),
	PQ_PUSH_SCOPE     = (1 << 2),
	PQ_COMPILER_MACRO = (1 << 3),
};

/**
 * Function metadata, used by every Function type in pq.
 */
typedef struct pq_function_metadata {
	pq_type *signature;
	uint8_t argnum;
	uint8_t flags;
} pq_function_metadata;

/**
 * Functions defined in pq: code defined by Lists.
 */
typedef struct pq_function {
	pq_function_metadata header;
	pq_list code;
} pq_function;

/**
 * Registers a pq Function into pq Context.
 */
pq_value *pq_register_function(pq_context *ctx, const char *name, pq_list code, uint8_t argnum, uint8_t is_variadic);

/// C Function prototype.
typedef pq_value *(*pq_c_function_ptr)(pq_context *ctx, int argc, pq_value **argv);
/// C Compiler Macro prototype.
typedef pq_value *(*pq_compiler_macro_ptr)(pq_context *ctx, jit_function_t jit_function, int argc, pq_value **argv);

/**
 * Functions defined in C, receiving pq Values in the argv array.
 */
typedef struct pq_c_function {
	pq_function_metadata header;
	union {
		pq_c_function_ptr function_ptr;
		pq_compiler_macro_ptr macro_ptr;
	} callable;
} pq_c_function;
/**
 * Registers a C Function into pq Context.
 */
pq_value *pq_register_c_function(pq_context *ctx, const char *name, pq_c_function_ptr func, uint8_t argnum, enum pq_function_flags flags);


/**
 * Try to call the `func` value, which should be callable (either a function, macro or type).
 *
 * @param ctx  Pq Context.
 * @param func Function value to be called.
 * @param argc Number of arguments.
 * @param argv Array of arguments to be passed.
 *
 * @return Value returned from the function, or an Error Value if there is any.
 */
pq_value *pq_call(pq_context *ctx, pq_value *func, int argc, pq_value **argv);
pq_value *pq_vcall(pq_context *ctx, pq_value *func, int argc, ...);
/**
 * Mark a value to be returned by a function.
 *
 * This is used on to pop the Scope pushed by `pq_call`, marking the return
 * Value, so that previous Scope own it.
 *
 * @return `ret`, so you can `return pq_return(ctx, val);`
 */
pq_value *pq_return(pq_context *ctx, pq_value *ret);

#endif

