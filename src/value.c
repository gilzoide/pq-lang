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

#include <string.h>

pq_value *pq_value_error(pq_context *ctx, const char *msg) {
	pq_value *val;
	if(val = pq_context_new_value(ctx)) {
		val->type = ctx->builtin_types._error;
		val->data = strdup(msg);
	}
	return val;
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
		case 1: type = ctx->builtin_types._i1; break;
		case 8: type = ctx->builtin_types._i8; break;
		case 16: type = ctx->builtin_types._i16; break;
		case 32: type = ctx->builtin_types._i32; break;
		case 64: type = ctx->builtin_types._i64; break;
		default: return pq_value_error(ctx, "Invalid number of bits for integer value: should be 1, 8, 16, 32 or 64");
	}
	if(val = pq_context_new_value(ctx)) {
		val->type = type;
		val->data = LLVMConstInt(type->data, i, 1);
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

