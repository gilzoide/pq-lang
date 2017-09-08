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

#include <pq/builtin.h>
#include <pq/context.h>
#include <pq.h>

#include <stdio.h>

int pq_register_builtin(pq_context *ctx) {
	return pq_register_builtin_types(ctx) &&
	       pq_register_builtin_values(ctx) &&
	       pq_register_builtin_functions(ctx);   
}

int pq_register_builtin_values(pq_context *ctx) {
	pq_value *nil_val;
	if(nil_val = pq_context_new_value(ctx)) {
		nil_val->type = ctx->builtin_types._nil;
		nil_val->data = NULL;
		ctx->builtin_values._nil = nil_val;
	}
}

////////////////////////////////////////////////////////////////////////////////
//  Builtin value destructors
////////////////////////////////////////////////////////////////////////////////
static void destroy_type(pq_context *ctx, void *data) {
	pq_type *type = data;
	free(type->name);
	if(type->self_destructor) {
		type->self_destructor(ctx, type->data);
	}
	free(type);
}
static void destroy_cons(pq_context *ctx, void *data) {
	pq_cons_cell *cell = data;
	pq_context_release_value(ctx, cell->first);
	pq_context_release_value(ctx, cell->second);
	free(cell);
}
static void destroy_error(pq_context *ctx, void *data) {
	free(data);
}
static void destroy_function(pq_context *ctx, void *data) {
	free(data);
}
////////////////////////////////////////////////////////////////////////////////

int pq_register_builtin_types(pq_context *ctx) {
	pq_value *type_val;
	if(type_val = pq_register_type(ctx, "type", PQ_TYPE, NULL, NULL, &destroy_type, NULL)) {
		type_val->type = ctx->builtin_types._type = type_val->data;
	}
	else {
		return 0;
	}

#define register_type(builtin_var, name, kind, ext, self_dtor, val_dtor, d) \
	if(type_val = pq_register_type(ctx, name, kind, ext, self_dtor, val_dtor, d)) { \
		ctx->builtin_types. builtin_var = type_val->data; \
	} \
	else return 0

	register_type(_error, "error-type", PQ_ERROR, NULL, NULL, &destroy_error, NULL);
	register_type(_cons_cell, "cons-type", PQ_CONS_CELL, NULL, NULL, &destroy_cons, NULL);
	register_type(_scope, "scope-type", PQ_SCOPE, NULL, NULL, (pq_destructor) &pq_scope_destroy, NULL);
	register_type(_nil, "nil-type", PQ_NIL, NULL, NULL, NULL, NULL);

	register_type(_function, "function-type", PQ_FUNCTION, NULL, NULL, &destroy_function, NULL);

	register_type(_bool, "bool", PQ_LLVM, NULL, NULL, NULL, LLVMInt1TypeInContext(ctx->llvm));
	register_type(_i8, "i8", PQ_LLVM, NULL, NULL, NULL, LLVMInt8TypeInContext(ctx->llvm));
	register_type(_i16, "i16", PQ_LLVM, NULL, NULL, NULL, LLVMInt16TypeInContext(ctx->llvm));
	register_type(_i32, "i32", PQ_LLVM, NULL, NULL, NULL, LLVMInt32TypeInContext(ctx->llvm));
	register_type(_i64, "i64", PQ_LLVM, NULL, NULL, NULL, LLVMInt64TypeInContext(ctx->llvm));
	register_type(_i128, "i128", PQ_LLVM, NULL, NULL, NULL, LLVMInt128TypeInContext(ctx->llvm));
	register_type(_u8, "u8", PQ_LLVM, NULL, NULL, NULL, LLVMInt8TypeInContext(ctx->llvm));
	register_type(_u16, "u16", PQ_LLVM, NULL, NULL, NULL, LLVMInt16TypeInContext(ctx->llvm));
	register_type(_u32, "u32", PQ_LLVM, NULL, NULL, NULL, LLVMInt32TypeInContext(ctx->llvm));
	register_type(_u64, "u64", PQ_LLVM, NULL, NULL, NULL, LLVMInt64TypeInContext(ctx->llvm));
	register_type(_u128, "u128", PQ_LLVM, NULL, NULL, NULL, LLVMInt128TypeInContext(ctx->llvm));

	register_type(_half, "half", PQ_LLVM, NULL, NULL, NULL, LLVMHalfTypeInContext(ctx->llvm));
	register_type(_float, "float", PQ_LLVM, NULL, NULL, NULL, LLVMFloatTypeInContext(ctx->llvm));
	register_type(_double, "double", PQ_LLVM, NULL, NULL, NULL, LLVMDoubleTypeInContext(ctx->llvm));
#undef register_type
}

////////////////////////////////////////////////////////////////////////////////
//  Builtin functions
////////////////////////////////////////////////////////////////////////////////
static pq_value *print_version(pq_context *ctx, int argc, pq_value *argv) {
	puts("pq version: " PQ_VERSION);
	return pq_value_nil(ctx);
}
////////////////////////////////////////////////////////////////////////////////

int pq_register_builtin_functions(pq_context *ctx) {
	pq_register_c_function(ctx, "print-version", &print_version, 0, 0);
}

