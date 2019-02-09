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

#include "core.h"

#include <pq/assert.h>
#include <pq/context.h>
#include <pq/value.h>

#include <string.h>

static pq_value *_let(pq_context *ctx, int argc, pq_value **argv) {
	pq_symbol sym;
	pq_value *val = pq_eval(ctx, argv[1]);
	pq_assert_not_error(val);
	switch(argv[0]->type->kind) {
		case PQ_SYMBOL:
			sym = pq_value_get_data_as(argv[0], pq_symbol);
			pq_context_set_symbol(ctx, sym, val);
			break;

		default:
			return pq_value_ferror(ctx, "Invalid argument 1: expected symbol, found %s",
					argv[0]->type->name);
	}
	return val;
}

static pq_value *_ref(pq_context *ctx, int argc, pq_value **argv) {
	pq_assert_arg_type(ctx, argv, 0, symbol);
	pq_value *val = pq_eval(ctx, argv[0]);
	pq_assert_not_error(val);
	return pq_value_pointer_for_value(ctx, val);
}

static pq_value *_set(pq_context *ctx, int argc, pq_value **argv) {
	pq_value *dst = argv[0];

	if(pq_is_symbol(dst)) {
		dst = pq_eval(ctx, dst);
		pq_assert_not_error(dst);
	}
	dst = pq_eval(ctx, dst);
	pq_assert_not_error(dst);

	pq_value *src = pq_eval(ctx, argv[1]);
	pq_assert_not_error(src);

	void *pointee;
	pq_type *pointee_type;

	switch(dst->type->kind) {
		case PQ_POINTER:
			pointee_type = pq_type_get_underlying_type(dst->type);
			pointee = pq_value_get_data_as(dst, void *);
			dst = src;
			break;

		default:
			pointee_type = dst->type;
			pointee = pq_value_get_data(dst);
			break;
	}
	if(pointee_type != src->type) {
		return pq_value_error(ctx, "Incompatible types for `set`");
	}
	memcpy(pointee, pq_value_get_data(src), pq_type_get_value_size(pointee_type));

	return dst;
}

int pq_register_core_value(pq_context *ctx) {
	return pq_register_c_function(ctx, "let", &_let, 2, 0)
	       && pq_register_c_function(ctx, "ref", &_ref, 1, 0)
	       && pq_register_c_function(ctx, "set", &_set, 2, 0)
	       && pq_register_c_function(ctx, "tuple", &pq_value_tuple_from_values, 1, PQ_EVAL_ARGS | PQ_VARIADIC);
}

