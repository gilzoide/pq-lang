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

#include <pq/value.h>
#include <pq/context.h>
#include <pq/function.h>

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#define PQ_FERROR_BUFFER_SIZE 256

////////////////////////////////////////////////////////////////////////////////
//  Value constructors
////////////////////////////////////////////////////////////////////////////////
pq_value *pq_value_error(pq_context *ctx, const char *msg) {
	pq_value *val;
	if(val = pq_new_value(ctx, char *)) {
		val->type = ctx->type_manager._error;
		pq_value_get_data_as(val, char *) = strdup(msg);
	}
	return val;
}

pq_value *pq_value_ferror(pq_context *ctx, const char *fmt, ...) {
	static char ferror_buffer[PQ_FERROR_BUFFER_SIZE];
	va_list args;
	va_start(args, fmt);
	vsnprintf(ferror_buffer, PQ_FERROR_BUFFER_SIZE, fmt, args);
	va_end(args);
	return pq_value_error(ctx, ferror_buffer);
}

pq_value *pq_value_from_type(pq_context *ctx, pq_type *type) {
	pq_value *val;
	if(val = pq_new_value(ctx, pq_type *)) {
		val->type = ctx->type_manager._type;
		pq_value_get_data_as(val, pq_type *) = type;
	}
	return val;
}

pq_value *pq_value_from_scope(pq_context *ctx, pq_scope *s) {
	pq_value *val;
	if(val = pq_new_value(ctx, pq_scope *)) {
		val->type = ctx->type_manager._scope;
		pq_value_get_data_as(val, pq_scope *) = s;
	}
	return val;
}

pq_value *pq_value_from_i8(pq_context *ctx, int8_t i) {
	pq_value *val;
	if(val = pq_new_value(ctx, int8_t)) {
		val->type = ctx->type_manager._i8;
		pq_value_get_data_as(val, int8_t) = i;
	}
	return val;
}

pq_value *pq_value_from_i16(pq_context *ctx, int16_t i) {
	pq_value *val;
	if(val = pq_new_value(ctx, int16_t)) {
		val->type = ctx->type_manager._i16;
		pq_value_get_data_as(val, int16_t) = i;
	}
	return val;
}

pq_value *pq_value_from_i32(pq_context *ctx, int32_t i) {
	pq_value *val;
	if(val = pq_new_value(ctx, int32_t)) {
		val->type = ctx->type_manager._i32;
		pq_value_get_data_as(val, int32_t) = i;
	}
	return val;
}

pq_value *pq_value_from_i64(pq_context *ctx, int64_t i) {
	pq_value *val;
	if(val = pq_new_value(ctx, int64_t)) {
		val->type = ctx->type_manager._i64;
		pq_value_get_data_as(val, int64_t) = i;
	}
	return val;
}

intmax_t pq_value_as_int(pq_value *val) {
	switch(pq_type_get_value_size(val->type)) {
		case 1: return (intmax_t) pq_value_get_data_as(val, int8_t);
		case 2: return (intmax_t) pq_value_get_data_as(val, int16_t);
		case 4: return (intmax_t) pq_value_get_data_as(val, int32_t);
		case 8: return (intmax_t) pq_value_get_data_as(val, int64_t);
		default: return 0;
	}
}

pq_value *pq_value_from_u8(pq_context *ctx, uint8_t u) {
	pq_value *val;
	if(val = pq_new_value(ctx, uint8_t)) {
		val->type = ctx->type_manager._u8;
		pq_value_get_data_as(val, uint8_t) = u;
	}
	return val;
}

pq_value *pq_value_from_u16(pq_context *ctx, uint16_t u) {
	pq_value *val;
	if(val = pq_new_value(ctx, uint16_t)) {
		val->type = ctx->type_manager._u16;
		pq_value_get_data_as(val, uint16_t) = u;
	}
	return val;
}

pq_value *pq_value_from_u32(pq_context *ctx, uint32_t u) {
	pq_value *val;
	if(val = pq_new_value(ctx, uint32_t)) {
		val->type = ctx->type_manager._u32;
		pq_value_get_data_as(val, uint32_t) = u;
	}
	return val;
}

pq_value *pq_value_from_u64(pq_context *ctx, uint64_t u) {
	pq_value *val;
	if(val = pq_new_value(ctx, uint64_t)) {
		val->type = ctx->type_manager._u64;
		pq_value_get_data_as(val, uint64_t) = u;
	}
	return val;
}

uintmax_t pq_value_as_uint(pq_value *val) {
	switch(pq_type_get_value_size(val->type)) {
		case 1: return (uintmax_t) pq_value_get_data_as(val, uint8_t);
		case 2: return (uintmax_t) pq_value_get_data_as(val, uint16_t);
		case 4: return (uintmax_t) pq_value_get_data_as(val, uint32_t);
		case 8: return (uintmax_t) pq_value_get_data_as(val, uint64_t);
		default: return 0;
	}
}

double pq_value_as_double(pq_value *val) {
	switch(pq_type_get_value_size(val->type)) {
		case sizeof(float): return (double) pq_value_get_data_as(val, float);
		case sizeof(double): return (double) pq_value_get_data_as(val, double);
		default: return NAN;
	}
}

pq_value *pq_value_from_float(pq_context *ctx, float f) {
	pq_value *val;
	if(val = pq_new_value(ctx, float)) {
		val->type = ctx->type_manager._float;
		pq_value_get_data_as(val, float) = f;
	}
	return val;
}

pq_value *pq_value_from_double(pq_context *ctx, double f) {
	pq_value *val;
	if(val = pq_new_value(ctx, double)) {
		val->type = ctx->type_manager._double;
		pq_value_get_data_as(val, double) = f;
	}
	return val;
}

pq_value *pq_value_from_string(pq_context *ctx, const char *str) {
	pq_value *val;
	if(val = pq_new_value(ctx, char *)) {
		val->type = ctx->type_manager._string;
		pq_value_get_data_as(val, char *) = strdup(str);
	}
	return val;
}

pq_value *pq_value_from_lstring(pq_context *ctx, const char *str, size_t n) {
	pq_value *val;
	if(val = pq_new_value(ctx, char *)) {
		val->type = ctx->type_manager._string;
		pq_value_get_data_as(val, char *) = strndup(str, n);
	}
	return val;
}

pq_value *pq_value_from_symbol(pq_context *ctx, pq_symbol symbol) {
	pq_value *val;
	if(val = pq_new_value(ctx, pq_symbol)) {
		val->type = ctx->type_manager._symbol;
		pq_value_get_data_as(val, pq_symbol) = symbol;
	}
	return val;
}

pq_value *pq_value_from_list(pq_context *ctx, pq_list lst) {
	pq_value *val;
	if(val = pq_new_value(ctx, pq_list)) {
		val->type = ctx->type_manager._list;
		pq_value_get_data_as(val, pq_list) = lst;
	}
	return val;
}

pq_value *pq_value_nil(pq_context *ctx) {
	return ctx->builtin_values._nil;
}

pq_value *pq_value_from_c_function(pq_context *ctx, pq_c_function_ptr fptr, uint8_t argnum, enum pq_function_flags flags) {
	pq_value *val;
	if(val = pq_new_value(ctx, pq_c_function)) {
		val->type = ctx->type_manager._c_function;
		pq_c_function *cfunc = pq_value_get_data(val);
		cfunc->header.argnum = argnum;
		cfunc->header.flags = flags;
		cfunc->callable.function_ptr = fptr;
	}
	return val;
}

pq_value *pq_value_from_compiler_macro(pq_context *ctx, pq_compiler_macro_ptr macro_ptr, uint8_t argnum, enum pq_function_flags flags) {
	pq_value *val;
	if(val = pq_new_value(ctx, pq_c_function)) {
		val->type = ctx->type_manager._c_function;
		pq_c_function *cfunc = pq_value_get_data(val);
		cfunc->header.argnum = argnum;
		cfunc->header.flags = flags;
		cfunc->callable.macro_ptr = macro_ptr;
	}
	return val;
}

pq_value *pq_value_from_native_function(pq_context *ctx, void *fptr, pq_type *signature) {
	pq_value *val;
	if(signature == NULL || signature->kind != PQ_SIGNATURE) {
		val = pq_value_error(ctx, "Native functions need a signature type");
	}
	else if(val = pq_new_value(ctx, pq_native_function)) {
		val->type = ctx->type_manager._native_function;
		pq_native_function *native_func = pq_value_get_data(val);
		native_func->header.signature = signature;
		native_func->header.argnum = jit_type_num_params(jit_type_remove_tags(signature->jit_type));
		native_func->header.flags = (jit_type_get_abi(signature->jit_type) == jit_abi_vararg ? PQ_VARIADIC : 0) | PQ_EVAL_ARGS;
		native_func->function_ptr = fptr;
	}
	return val;
}

pq_value *pq_value_from_code(pq_context *ctx, pq_list code, uint8_t argnum, enum pq_function_flags flags) {
	pq_value *val;
	if(val = pq_new_value(ctx, pq_function)) {
		val->type = ctx->type_manager._function;
		pq_function *func = pq_value_get_data(val);
		func->header.argnum = argnum;
		func->header.flags = flags;
		func->code = code;
	}
	return val;
}

////////////////////////////////////////////////////////////////////////////////
//  Variables
////////////////////////////////////////////////////////////////////////////////
pq_value *pq_new_variable(pq_context *ctx, pq_type *type) {
	if(type->kind == PQ_NIL) {
		return pq_value_nil(ctx);
	}
	pq_value *variable;
	if(variable = pq_new_value_with_size(ctx, pq_type_get_value_size(type))) {
		variable->type = type;
	}
	return variable;
}

////////////////////////////////////////////////////////////////////////////////
//  Value Type Checks
////////////////////////////////////////////////////////////////////////////////
int pq_is_int(pq_value *val) {
	int kind = val->type->kind;
	return kind == PQ_INT;
}

int pq_is_float(pq_value *val) {
	int kind = val->type->kind;
	return kind == PQ_FLOAT;
}

int pq_is_error(pq_value *val) {
	int kind = val->type->kind;
	return kind == PQ_ERROR;
}

int pq_is_callable(pq_value *val) {
	int kind = val->type->kind;
	return kind == PQ_FUNCTION
	       | kind == PQ_C_FUNCTION
	       | kind == PQ_NATIVE_FUNCTION
	       | kind == PQ_TYPE;
}

int pq_is_nil(pq_value *val) {
	int kind = val->type->kind;
	return kind == PQ_NIL;
}

int pq_is_symbol(pq_value *val) {
	int kind = val->type->kind;
	return kind == PQ_SYMBOL;
}

int pq_is_string(pq_value *val) {
	int kind = val->type->kind;
	return kind == PQ_STRING;
}

int pq_true(pq_value *val) {
	return !pq_false(val);
}

int pq_false(pq_value *val) {
	if(val == NULL) return 1;
	switch(val->type->kind) {
		case PQ_NIL:
		case PQ_ERROR:
			return 1;

		case PQ_INT:
			return pq_value_get_data_as(val, intmax_t) == 0;

		default:
			return 0;
	}
}

////////////////////////////////////////////////////////////////////////////////
//  General operations
////////////////////////////////////////////////////////////////////////////////
void pq_fprint(pq_context *ctx, pq_value *val, FILE *output) {
	switch(val->type->kind) {
		case PQ_NIL:
			fputs("nil", output);
			break;

		case PQ_SYMBOL:
			fprintf(output, "%s", pq_string_from_symbol(ctx, pq_value_get_data_as(val, pq_symbol)));
			break;

		case PQ_STRING:
			fprintf(output, "\"%s\"", pq_value_get_data_as(val, char *));
			break;

		case PQ_INT:
			fprintf(output, "%ld", pq_value_as_int(val));
			break;

		case PQ_FLOAT:
			fprintf(output, "%g", pq_value_as_double(val));
			break;

		case PQ_LIST: {
				pq_list lst = pq_value_get_data_as(val, pq_list);
				int i;
				fputc('(', output);
				for(i = 0; i < lst.size; i++) {
					pq_fprint(ctx, lst.values[i], output);
					if(i < lst.size - 1) {
						fputc(' ', output);
					}
				}
				fputc(')', output);
			}
			break;

		case PQ_ERROR:
			fprintf(output, "Error: %s", pq_value_get_data_as(val, char *));
			break;

		case PQ_TYPE:
			fprintf(output, "%s", pq_value_get_data_as(val, pq_type *)->name);
			break;

		default:
			fprintf(output, "[no print for type \"%s\"]", val->type->name);
			break;
	}
}

void pq_print(pq_context *ctx, pq_value *val) {
	pq_fprint(ctx, val, stdout);
}
