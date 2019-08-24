/*
 * Copyright 2017-2019 Gil Barbosa Reis <gilzoide@gmail.com>
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
#include <pq/value.h>

pq_value *pq_eval(pq_context *ctx, pq_value *val) {
	if(val == NULL) return pq_value_nil(ctx);
	switch(val->type->kind) {
		case PQ_SYMBOL:
			return pq_context_get_symbol(ctx, pq_value_get_data_as(val, pq_symbol));

		case PQ_LIST: {
			pq_list lst = pq_value_get_data_as(val, pq_list);
			if(lst.size == 0) {
				return pq_value_nil(ctx);
			}
			pq_value *func = pq_eval(ctx, lst.values[0]);
			pq_assert_not_error(func);
			int argc = lst.size - 1;
			pq_value *ret = pq_call(ctx, func, argc, lst.values + 1);
			return ret;
		}

		default:
			return val;
	}
}

