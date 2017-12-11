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

#ifndef __PQ_VALUE_H__
#define __PQ_VALUE_H__

#include "cons.h"
#include "type.h"
#include "function.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

/**
 * Pq Value: a value with a Type, and data.
 *
 * The data section of a Value is expected to be right after this header in
 * memory, so Values can be freed more easily. The data section
 * may contain anything. The contents are defined by the Type.
 */
typedef struct pq_value {
	pq_type *type;
	unsigned int parent_scope;
} pq_value;

/**
 * Get the data section of a Value, which may contain anything.
 *
 * Just be sure you have allocated the right amount of memory.
 */
static inline void *pq_value_get_data(pq_value *val) {
	return (void *) &val[1];
}
/**
 * Facility to get a Value data section derreferenced and cast to some C type.
 *
 * This may also be used directly for setting the Value data.
 */
#define pq_value_get_data_as(val, c_type) (*((c_type *) pq_value_get_data(val)))

// Forward declarations
typedef struct pq_context pq_context;
typedef struct pq_scope pq_scope;

// Value constructors
pq_value *pq_value_error(pq_context *ctx, const char *msg);
pq_value *pq_value_ferror(pq_context *ctx, const char *fmt, ...);

pq_value *pq_value_from_type(pq_context *ctx, pq_type *t);
pq_value *pq_value_from_scope(pq_context *ctx, pq_scope *s);

pq_value *pq_value_from_int(pq_context *ctx, intmax_t i, unsigned numbits);
pq_value *pq_value_from_uint(pq_context *ctx, uintmax_t u, unsigned numbits);
pq_value *pq_value_from_string(pq_context *ctx, const char *str);
pq_value *pq_value_from_lstring(pq_context *ctx, const char *str, size_t n);
pq_value *pq_value_cons(pq_context *ctx, pq_value *first, pq_value *second);

pq_value *pq_value_nil(pq_context *ctx);

pq_value *pq_value_from_c_function(pq_context *ctx, pq_c_function_ptr fptr, uint8_t argnum, uint8_t is_variadic, uint8_t is_macro);
pq_value *pq_value_from_code(pq_context *ctx, pq_value *code, uint8_t argnum, uint8_t is_variadic);

// Value type checks

/// Checks if a Value is an integer.
int pq_is_int(pq_value *val);
int pq_is_error(pq_value *val);
int pq_is_callable(pq_value *val);
int pq_is_nil(pq_value *val);


// General operations
void pq_fprint(pq_context *ctx, pq_value *val, FILE *output);
void pq_print(pq_context *ctx, pq_value *val);

#endif

