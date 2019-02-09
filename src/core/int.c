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

#include <pq/context.h>
#include <pq/assert.h>

static pq_value *_int_plus(pq_context *ctx, int argc, pq_value **argv) {
	int i;
	intmax_t result = pq_value_as_int(argv[0]);
	for(i = 1; i < argc; i++) {
		pq_assert_arg_type(ctx, argv, i, int);
		result += pq_value_as_int(argv[i]);
	}
	return pq_value_from_i64(ctx, result);
}

static pq_value *_int_minus(pq_context *ctx, int argc, pq_value **argv) {
	int i;
	intmax_t result = pq_value_as_int(argv[0]);
	for(i = 1; i < argc; i++) {
		pq_assert_arg_type(ctx, argv, i, int);
		result -= pq_value_as_int(argv[i]);
	}
	if(argc == 1) result = -result;
	return pq_value_from_i64(ctx, result);
}

int pq_register_core_int(pq_context *ctx) {
	pq_type *ret_type, *arg_types[1];
    pq_type *int_type_kind = pq_get_type_kind_type(ctx, PQ_INT);
	int i, result = 1;

    ret_type = pq_get_builtin_type(ctx, PQ_TYPE_I64);
    arg_types[0] = int_type_kind;
    result = pq_register_typed_c_function(ctx, "+", &_int_plus, ret_type, 1, arg_types, PQ_EVAL_ARGS | PQ_VARIADIC) != NULL
             && pq_register_typed_c_function(ctx, "-", &_int_minus, ret_type, 1, arg_types, PQ_EVAL_ARGS | PQ_VARIADIC) != NULL;

	return result;
}

