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

#include <pq/function.h>
#include <pq/assert.h>
#include <pq/context.h>

pq_value *pq_register_function(pq_context *ctx, const char *name, pq_list args, pq_list code, enum pq_function_flags flags) {
	pq_value *func_val;
	if(func_val = pq_value_from_code(ctx, args, code, flags)) {
		pq_context_set_function(ctx, name, func_val);
	}
	return func_val;
}

pq_value *pq_register_c_function(pq_context *ctx, const char *name, pq_c_function_ptr func, uint8_t argnum, enum pq_function_flags flags) {
	pq_value *func_val;
	if(func_val = pq_value_from_c_function(ctx, func, argnum, flags)) {
		pq_context_set_function(ctx, name, func_val);
	}
	return func_val;
}

pq_value *pq_register_typed_c_function(pq_context *ctx, const char *name, pq_c_function_ptr func, pq_type *return_type, uint8_t argnum, pq_type **argtypes, enum pq_function_flags flags) {
	pq_value *func_val;
	if(func_val = pq_value_from_typed_c_function(ctx, func, return_type, argnum, argtypes, flags)) {
		pq_context_set_function(ctx, name, func_val);
	}
	return func_val;
}

pq_value *pq_register_native_function(pq_context *ctx, const char *name, void *fptr, pq_type *signature) {
	pq_value *func_val;
	if(func_val = pq_value_from_native_function(ctx, fptr, signature)) {
		pq_context_set_function(ctx, name, func_val);
	}
	return func_val;
}

pq_value *pq_register_compiler_macro(pq_context *ctx, const char *name, pq_compiler_macro_ptr macro, uint8_t argnum, enum pq_function_flags flags) {
	pq_value *func_val;
	if(func_val = pq_value_from_compiler_macro(ctx, macro, argnum, flags)) {
		pq_context_set_function(ctx, name, func_val);
	}
	return func_val;
}

pq_value *pq_return(pq_context *ctx, pq_value *ret) {
	pq_scope *top = pq_scope_queue_pop(&ctx->scopes);
	if(top == NULL) {
		return pq_value_error(ctx, "cannot pop scope from empty queue");
	}
	else if(ret && ret->parent_scope >= ctx->scopes.scopes.size) {
		ret->parent_scope--;
		if(!pq_scope_mark_value_for_destruction(top - 1, ret)) {
			return pq_value_error(ctx, "memory error on scope destruction");
		}
	}
	pq_scope_destroy(ctx, top);
	return ret;
}

