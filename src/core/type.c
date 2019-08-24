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

#include <pq/assert.h>
#include <pq/function.h>
#include <pq/type.h>
#include <pq/value.h>

static pq_value *_pq_size_of(pq_context *ctx, int argc, pq_value **argv) {
    pq_type *type = argv[0]->type;
    return pq_value_from_i64(ctx, pq_type_get_value_size(type));
}

static pq_value *_pq_type_size(pq_context *ctx, int argc, pq_value **argv) {
    pq_assert_arg_type(ctx, argv, 0, type);
    pq_type *type = pq_value_get_data_as(argv[0], pq_type *);
    return pq_value_from_i64(ctx, pq_type_get_value_size(type));
}

/* static pq_value *_pq_type_create_struct(pq_context *ctx, int argc, pq_value **argv) { */
/*  */
/* } */

int pq_register_core_type(pq_context *ctx) {
    pq_type *i64_type = pq_get_builtin_type(ctx, PQ_TYPE_I64);
    pq_type *type_types = pq_get_type_kind_type(ctx, PQ_TYPE);
    return pq_register_c_function(ctx, "size-of", &_pq_size_of, 1, PQ_EVAL_ARGS)
        && pq_register_typed_c_function(ctx, "size-of", &_pq_type_size, i64_type, 1, (pq_type *[]){ type_types }, PQ_EVAL_ARGS)
        && pq_register_typed_c_function(ctx, "size-of", &_pq_type_size, i64_type, 1, (pq_type *[]){ type_types }, PQ_EVAL_ARGS | PQ_VARIADIC)
        && 1;
}

