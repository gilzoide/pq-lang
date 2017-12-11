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

#include <pq/value.h>
#include <pq/context.h>
#include <pq/function.h>

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
		val->type = ctx->builtin_types._error;
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

pq_value *pq_value_from_type(pq_context *ctx, pq_type *t) {
	pq_value *val;
	if(val = pq_new_value(ctx, pq_type *)) {
		val->type = ctx->builtin_types._type;
		pq_value_get_data_as(val, pq_type *) = t;
	}
	return val;
}

pq_value *pq_value_from_scope(pq_context *ctx, pq_scope *s) {
	pq_value *val;
	if(val = pq_new_value(ctx, pq_scope *)) {
		val->type = ctx->builtin_types._scope;
		pq_value_get_data_as(val, pq_scope *) = s;
	}
	return val;
}

pq_value *pq_value_from_int(pq_context *ctx, intmax_t i, unsigned numbits) {
	pq_value *val;
	pq_type *type;
	switch(numbits) {
		case 1: type = ctx->builtin_types._bool; break;
		case 8: type = ctx->builtin_types._i8; break;
		case 16: type = ctx->builtin_types._i16; break;
		case 32: type = ctx->builtin_types._i32; break;
		case 64: type = ctx->builtin_types._i64; break;
		case 128: type = ctx->builtin_types._i128; break;
		default: return pq_value_ferror(ctx, "Invalid number of bits for integer value: expected 1, 8, 16, 32, 64 or 128; found %d", numbits);
	}
	if(val = pq_new_value(ctx, intmax_t)) {
		val->type = type;
		pq_value_get_data_as(val, intmax_t) = i;
	}
	return val;
}

pq_value *pq_value_from_uint(pq_context *ctx, uintmax_t u, unsigned numbits) {
	pq_value *val;
	pq_type *type;
	switch(numbits) {
		case 1: type = ctx->builtin_types._bool; break;
		case 8: type = ctx->builtin_types._u8; break;
		case 16: type = ctx->builtin_types._u16; break;
		case 32: type = ctx->builtin_types._u32; break;
		case 64: type = ctx->builtin_types._u64; break;
		case 128: type = ctx->builtin_types._u128; break;
		default: return pq_value_ferror(ctx, "Invalid number of bits for unsigned integer value: expected 1, 8, 16, 32, 64 or 128; found %d", numbits);
	}
	if(val = pq_new_value(ctx, uintmax_t)) {
		val->type = type;
		pq_value_get_data_as(val, uintmax_t) = u;
	}
	return val;
}

pq_value *pq_value_from_string(pq_context *ctx, const char *str) {
	pq_value *val;
	if(val = pq_new_value(ctx, char *)) {
		val->type = ctx->builtin_types._symbol;
		pq_value_get_data_as(val, char *) = strdup(str);
	}
	return val;
}

pq_value *pq_value_from_lstring(pq_context *ctx, const char *str, size_t n) {
	pq_value *val;
	if(val = pq_new_value(ctx, char *)) {
		val->type = ctx->builtin_types._symbol;
		pq_value_get_data_as(val, char *) = strndup(str, n);
	}
	return val;
}

pq_value *pq_value_cons(pq_context *ctx, pq_value *first, pq_value *second) {
	pq_value *val;
	if(val = pq_new_value(ctx, pq_cons_cell)) {
		val->type = ctx->builtin_types._cons_cell;
		pq_cons_cell *cell = pq_value_get_data(val);
		cell->first = first;
		cell->second = second;
	}
	return val;
}

pq_value *pq_value_nil(pq_context *ctx) {
	return ctx->builtin_values._nil;
}

pq_value *pq_value_from_c_function(pq_context *ctx, pq_c_function_ptr fptr, uint8_t argnum, uint8_t is_variadic, uint8_t is_macro) {
	pq_value *val;
	if(val = pq_new_value(ctx, pq_c_function)) {
		val->type = is_macro ? ctx->builtin_types._c_macro : ctx->builtin_types._c_function;
		pq_c_function *cfunc = pq_value_get_data(val);
		cfunc->header.argnum = argnum;
		cfunc->header.is_variadic = is_variadic;
		cfunc->fptr = fptr;
	}
	return val;
}

pq_value *pq_value_from_code(pq_context *ctx, pq_value *code, uint8_t argnum, uint8_t is_variadic) {
	pq_value *val;
	if(val = pq_new_value(ctx, pq_function)) {
		val->type = ctx->builtin_types._function;
		pq_function *func = pq_value_get_data(val);
		func->header.argnum = argnum;
		func->header.is_variadic = is_variadic;
		func->code = code;
	}
	return val;
}

////////////////////////////////////////////////////////////////////////////////
//  Value Type Checks
////////////////////////////////////////////////////////////////////////////////
int pq_is_int(pq_value *val) {
	return val->type->kind == PQ_INT;
}

int pq_is_float(pq_value *val) {
	return val->type->kind == PQ_FLOAT;
}

int pq_is_error(pq_value *val) {
	return val->type->kind == PQ_ERROR;
}

int pq_is_callable(pq_value *val) {
	return val->type->kind == PQ_FUNCTION
	       || val->type->kind == PQ_MACRO
	       || val->type->kind == PQ_C_FUNCTION
	       || val->type->kind == PQ_C_MACRO
	       || val->type->kind == PQ_LLVM_MACRO
	       || val->type->kind == PQ_LLVM_FUNCTION;
}

int pq_is_nil(pq_value *val) {
	return val->type->kind == PQ_NIL;
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
			fprintf(output, "%s", pq_value_get_data_as(val, char *));
			break;

		case PQ_STRING:
			fprintf(output, "\"%s\"", pq_value_get_data_as(val, char *));
			break;

		case PQ_INT:
			fprintf(output, "%d", pq_value_get_data_as(val, intmax_t));
			break;

		case PQ_FLOAT:
			fprintf(output, "%f", pq_value_get_data_as(val, double));
			break;

		case PQ_CONS_CELL:
			fputc('(', output);
			pq_cons_cell *cell;
			while(1) {
				cell = pq_value_get_data(val);
				val = cell->second;
				pq_fprint(ctx, cell->first, output);
				if(pq_is_nil(val)) break;
				fputc(' ', output);
			}
			fputc(')', output);
			break;

		case PQ_ERROR:
			fprintf(output, "error: %s", pq_value_get_data_as(val, char *));
			break;

		default:
			fprintf(output, "[no print for type \"%s\"]", val->type->name);
	}
}

void pq_print(pq_context *ctx, pq_value *val) {
	pq_fprint(ctx, val, stdout);
}
