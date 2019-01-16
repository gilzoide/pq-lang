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
#include <pq/list.h>

static pq_value *_pq_list_at(pq_context *ctx, int argc, pq_value **argv) {
	pq_list lst = pq_value_get_data_as(argv[0], pq_list);
	int index = pq_value_as_int(argv[1]);
	pq_value *result = pq_list_value_at(lst, index);
	return result ? result : pq_value_ferror(ctx, "List index %d out of bounds for list of size %d", index, lst.size);
}

static pq_value *_pq_list_size(pq_context *ctx, int argc, pq_value **argv) {
	pq_list lst = pq_value_get_data_as(argv[0], pq_list);
	return pq_value_from_i32(ctx, lst.size);
}

static pq_value *_pq_list_slice(pq_context *ctx, int argc, pq_value **argv) {
	pq_list lst = pq_value_get_data_as(argv[0], pq_list);
	int from = pq_value_as_int(argv[1]);
	int to = argc > 2 ? pq_value_as_int(argv[2]) : lst.size;
	pq_list slice = pq_list_slice(lst, from, to);
	return slice.size ? pq_value_from_list(ctx, slice) : pq_value_nil(ctx);
}

static pq_value *_pq_list_each(pq_context *ctx, int argc, pq_value **argv) {
	pq_assert_arg_type(ctx, argv, 0, list);
	pq_assert_arg_type(ctx, argv, 1, callable);
	pq_list lst = pq_value_get_data_as(argv[0], pq_list);
	pq_value *callable = argv[1];
	int i;
	for(i = 0; i < lst.size; i++) {
		pq_assert_not_error(pq_call(ctx, callable, 1, lst.values + i));
	}
	return pq_value_nil(ctx);
}

int pq_register_core_list(pq_context *ctx) {
	pq_type *lst_type = pq_get_builtin_type(ctx, PQ_TYPE_LIST);
	pq_type *i32_type = pq_get_builtin_type(ctx, PQ_TYPE_I32);
	return pq_register_c_function(ctx, "list", &pq_value_list_from_values, 1, PQ_VARIADIC | PQ_EVAL_ARGS)
	    && pq_register_typed_c_function(ctx, "at", &_pq_list_at, NULL, 2, (pq_type*[]){ lst_type, i32_type }, PQ_EVAL_ARGS)
	    && pq_register_typed_c_function(ctx, "size", &_pq_list_size, i32_type, 1, (pq_type*[]){ lst_type }, PQ_EVAL_ARGS)
	    && pq_register_typed_c_function(ctx, "slice", &_pq_list_slice, lst_type, 2, (pq_type*[]){ lst_type, i32_type }, PQ_EVAL_ARGS)
	    && pq_register_typed_c_function(ctx, "slice", &_pq_list_slice, lst_type, 3, (pq_type*[]){ lst_type, i32_type, i32_type }, PQ_EVAL_ARGS)
	    && pq_register_c_function(ctx, "each", &_pq_list_each, 2, PQ_EVAL_ARGS);
}

