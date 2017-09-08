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

#include <pq/function.h>
#include <pq/context.h>

pq_value *pq_register_c_function(pq_context *ctx, const char *name, pq_c_function_ptr func, uint8_t argmin, uint8_t argmax) {
	pq_value *func_val;
	if(func_val = pq_value_from_c_function(ctx, func, argmin, argmax)) {
		pq_context_set(ctx, name, func_val);
	}
	return func_val;
}

pq_value *pq_call(pq_context *ctx, pq_value *func, int argc, pq_value *argv) {
	if(!func) {
		return pq_value_error(ctx, "Can't call a null value");
	}
	else {
		switch(func->type->kind) {
			case PQ_FUNCTION: ;
				pq_c_function *func_val = func->data;
				if(argc < (int) func_val->argmin || argc > (int) func_val->argmax) {
					return pq_value_ferror(ctx, "Expected between %u and %u arguments; found %d", func_val->argmin, func_val->argmax, argc);
				}
				return func_val->fptr(ctx, argc, argv);
			default:
				return pq_value_ferror(ctx, "Can't call a value of type \"%s\"", func->type->name);
		}
	}
}

