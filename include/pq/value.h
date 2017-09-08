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

#include "type.h"

#include <stdint.h>

/**
 * Pq Value: a value with a Type.
 */
typedef struct pq_value {
	pq_type *type;
	void *data;
	uint16_t parent_scope;
} pq_value;

// Forward declarations
typedef struct pq_context pq_context;
typedef struct pq_scope pq_scope;

pq_value *pq_value_error(pq_context *ctx, const char *msg);
pq_value *pq_value_ferror(pq_context *ctx, const char *fmt, ...);

pq_value *pq_value_from_type(pq_context *ctx, pq_type *t);
pq_value *pq_value_from_scope(pq_context *ctx, pq_scope *s);

pq_value *pq_value_from_int(pq_context *ctx, intmax_t i, unsigned numbits);
pq_value *pq_value_from_uint(pq_context *ctx, uintmax_t u, unsigned numbits);
pq_value *pq_value_cons(pq_context *ctx, pq_value *first, pq_value *second);

pq_value *pq_value_nil(pq_context *ctx);

/// C Function prototype.
typedef pq_value *(*pq_c_function_ptr)(pq_context *ctx, int argc, pq_value *argv);
pq_value *pq_value_from_c_function(pq_context *ctx, pq_c_function_ptr fptr, uint8_t argmin, uint8_t argmax);

#endif

