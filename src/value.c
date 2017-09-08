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

pq_value *pq_value_error(pq_context *ctx, const char *msg) {
	pq_value *val;
	if(val = pq_context_new_value(ctx)) {
		val->type = ctx->builtin_types._error;
		val->data = strdup(msg);
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
	if(val = pq_context_new_value(ctx)) {
		val->type = ctx->builtin_types._type;
		val->data = t;
	}
	return val;
}

pq_value *pq_value_from_scope(pq_context *ctx, pq_scope *s) {
	pq_value *val;
	if(val = pq_context_new_value(ctx)) {
		val->type = ctx->builtin_types._scope;
		val->data = s;
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
	if(val = pq_context_new_value(ctx)) {
		val->type = type;
		val->data = LLVMConstInt(type->data, i, 1);
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
	if(val = pq_context_new_value(ctx)) {
		val->type = type;
		val->data = LLVMConstInt(type->data, u, 0);
	}
	return val;
}

pq_value *pq_value_cons(pq_context *ctx, pq_value *first, pq_value *second) {
	pq_value *val;
	pq_cons_cell *cell;
	if(val = pq_context_new_value(ctx)) {
		if(cell = pq_context_new_cons_cell(ctx)) {
			val->type = ctx->builtin_types._cons_cell;
			val->data = cell;
		}
		else {
			pq_context_release_value(ctx, val);
			val = NULL;
		}
	}
	return val;
}

pq_value *pq_value_nil(pq_context *ctx) {
	return ctx->builtin_values._nil;
}

pq_value *pq_value_from_c_function(pq_context *ctx, pq_c_function_ptr fptr, uint8_t argmin, uint8_t argmax) {
	pq_value *val;
	if(val = pq_context_new_value(ctx)) {
		pq_c_function *func;
		if(func = malloc(sizeof(pq_c_function))) {
			func->fptr = fptr;
			func->argmin = argmin;
			func->argmax = argmax;
		}
		val->type = ctx->builtin_types._function;
		val->data = func;
	}
	return val;
}

