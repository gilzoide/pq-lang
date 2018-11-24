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

int pq_register_core_int(pq_context *ctx) {
	pq_type *ret_type, *arg_types[1];
	int i, result = 1;
	for(i = PQ_TYPE_I8; result && i <= PQ_TYPE_SYS_UNSIGNED_LONG; i++) {
		ret_type = pq_get_builtin_type(ctx, i);
		arg_types[0] = ret_type;
		result = pq_register_typed_c_function(ctx, "+", &_int_plus, ret_type, 1, arg_types, PQ_EVAL_ARGS | PQ_VARIADIC) != NULL;
	}
	return result;
}

