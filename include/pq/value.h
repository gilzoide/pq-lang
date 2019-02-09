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

#ifndef __PQ_VALUE_H__
#define __PQ_VALUE_H__

#include "list.h"
#include "function.h"
#include "overload.h"
#include "symbol.h"
#include "type.h"
#include "type_manager.h"

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
	uint8_t data[0];
} pq_value;

/**
 * Get the data section of a Value, which may contain anything.
 *
 * Just be sure you have allocated the right amount of memory.
 */
static inline void *pq_value_get_data(pq_value *val) {
	return (void *)val->data;
}
/**
 * Facility to get a Value data section derreferenced and cast to some C type.
 *
 * This may also be used directly for setting the Value data.
 */
#define pq_value_get_data_as(val, c_type) \
	(*((c_type *)pq_value_get_data(val)))

// Forward declarations
typedef struct pq_context pq_context;
typedef struct pq_scope pq_scope;

// Value constructors
pq_value *pq_value_error(pq_context *ctx, const char *msg);
pq_value *pq_value_ferror(pq_context *ctx, const char *fmt, ...);

pq_value *pq_value_from_type(pq_context *ctx, pq_type *type);
pq_value *pq_value_from_scope(pq_context *ctx, pq_scope *s);

pq_value *pq_value_from_bool(pq_context *ctx, int b);

pq_value *pq_value_from_i8(pq_context *ctx, int8_t i);
pq_value *pq_value_from_i16(pq_context *ctx, int16_t i);
pq_value *pq_value_from_i32(pq_context *ctx, int32_t i);
pq_value *pq_value_from_i64(pq_context *ctx, int64_t i);
pq_value *pq_value_from_int(pq_context *ctx, intmax_t i, enum pq_builtin_type iN);

pq_value *pq_value_from_u8(pq_context *ctx, uint8_t u);
pq_value *pq_value_from_u16(pq_context *ctx, uint16_t u);
pq_value *pq_value_from_u32(pq_context *ctx, uint32_t u);
pq_value *pq_value_from_u64(pq_context *ctx, uint64_t u);

pq_value *pq_value_from_float(pq_context *ctx, float f);
pq_value *pq_value_from_double(pq_context *ctx, double d);

pq_value *pq_value_from_string(pq_context *ctx, const char *str);
pq_value *pq_value_from_lstring(pq_context *ctx, const char *str, size_t n);
pq_value *pq_value_from_symbol(pq_context *ctx, pq_symbol symbol);
pq_value *pq_value_from_list(pq_context *ctx, pq_list lst);
pq_value *pq_value_list_from_values(pq_context *ctx, int size, pq_value **values);

pq_value *pq_value_from_pointer(pq_context *ctx, void *ptr, pq_type *pointee_type);
pq_value *pq_value_pointer_for_value(pq_context *ctx, pq_value *val);

pq_value *pq_value_nil(pq_context *ctx);

pq_value *pq_value_from_c_function(pq_context *ctx, pq_c_function_ptr fptr, uint8_t argnum, enum pq_function_flags flags);
pq_value *pq_value_from_typed_c_function(pq_context *ctx, pq_c_function_ptr fptr, pq_type *return_type, uint8_t argnum, pq_type **argtypes, enum pq_function_flags flags);
pq_value *pq_value_from_compiler_macro(pq_context *ctx, pq_compiler_macro_ptr macro_ptr, uint8_t argnum, enum pq_function_flags flags);
pq_value *pq_value_from_native_function(pq_context *ctx, void *fptr, pq_type *signature);
pq_value *pq_value_from_code(pq_context *ctx, pq_list args, pq_list code, enum pq_function_flags flags);
pq_value *pq_value_from_overload(pq_context *ctx, pq_overload overload);

pq_value *pq_value_tuple_from_values(pq_context *ctx, int size, pq_value **values);
pq_value *pq_value_tuple_from_list(pq_context *ctx, pq_list lst);

// Value native getters
int pq_value_as_bool(pq_value *val);
intmax_t pq_value_as_int(pq_value *val);
uintmax_t pq_value_as_uint(pq_value *val);
double pq_value_as_double(pq_value *val);
pq_symbol pq_value_as_symbol(pq_value *val);
pq_list pq_value_as_list(pq_value *val);

/// Create a new Value with uninitialised memory with the specified Type.
pq_value *pq_new_uninitialized_value(pq_context *ctx, pq_type *type);
/// Create a new Value with memory initialised from a source pointer with the specified Type.
pq_value *pq_new_initialized_value(pq_context *ctx, pq_type *type, void *src);

// Value type checks

/// Checks if a Value is an integer.
int pq_is_int(pq_value *val);
int pq_is_float(pq_value *val);
int pq_is_error(pq_value *val);
int pq_is_function(pq_value *val);
int pq_is_overload(pq_value *val);
int pq_is_callable(pq_value *val);
int pq_is_nil(pq_value *val);
int pq_is_symbol(pq_value *val);
int pq_is_string(pq_value *val);
int pq_is_list(pq_value *val);
int pq_is_pointer(pq_value *val);

// General operations
void pq_fprint(pq_context *ctx, pq_value *val, FILE *output);
void pq_print(pq_context *ctx, pq_value *val);

#endif

