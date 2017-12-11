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
	return pq_register_builtin_types(ctx)
	       && pq_register_builtin_values(ctx)
	       && pq_register_builtin_functions(ctx);   
}

int pq_register_builtin_values(pq_context *ctx) {
	pq_value *new_val;
	if(new_val = pq_new_value_with_size(ctx, 0)) {
		new_val->type = ctx->builtin_types._nil;
		ctx->builtin_values._nil = new_val;
		pq_context_set(ctx, "nil", new_val);
	}
	else return 0;

#define register_value(builtin_var, name, val) \
	if(new_val = val) { \
		ctx->builtin_values. builtin_var = new_val; \
		pq_context_set(ctx, name, new_val); \
	} \
	else return 0

	register_value(_false, "false", pq_value_from_int(ctx, 0, 1));
	register_value(_true, "true", pq_value_from_int(ctx, 1, 1));
#undef register_value
	return 1;
}

////////////////////////////////////////////////////////////////////////////////
//  Builtin value destructors
////////////////////////////////////////////////////////////////////////////////
static void _destroy_type(pq_context *ctx, void *data) {
	pq_type *type = *((pq_type **) data);
	free(type->name);
	if(type->self_destructor) {
		type->self_destructor(ctx, type->data);
	}
	free(type);
}
static void _free_data(pq_context *ctx, void *data) {
	free(*((void **) data));
}
static void _destroy_function(pq_context *ctx, void *data) {
	pq_function *f = data;
}
////////////////////////////////////////////////////////////////////////////////

int pq_register_builtin_types(pq_context *ctx) {
	pq_value *type_val;
	if(type_val = pq_register_type(ctx, "type", PQ_TYPE, NULL, (pq_destructor) &_destroy_type, NULL)) {
		type_val->type = ctx->builtin_types._type = pq_value_get_data_as(type_val, pq_type *);
	}
	else return 0;

#define register_type(builtin_var, name, kind, self_dtor, val_dtor, d) \
	if(type_val = pq_register_type(ctx, name, kind, self_dtor, val_dtor, d)) { \
		ctx->builtin_types. builtin_var = pq_value_get_data_as(type_val, pq_type *); \
	} \
	else return 0

	register_type(_error, "error-t", PQ_ERROR, NULL, &_free_data, NULL);
	register_type(_cons_cell, "cons-t", PQ_CONS_CELL, NULL, NULL, NULL);
	register_type(_scope, "scope-t", PQ_SCOPE, NULL, (pq_destructor) &pq_scope_destroy, NULL);
	register_type(_nil, "nil-t", PQ_NIL, NULL, NULL, NULL);
	register_type(_symbol, "symbol", PQ_SYMBOL, NULL, &_free_data, NULL);

	register_type(_function, "function-t", PQ_FUNCTION, NULL, (pq_destructor) &_destroy_function, NULL);
	register_type(_c_function, "c-function-t", PQ_C_FUNCTION, NULL, NULL, NULL);
	register_type(_c_macro, "c-macro-t", PQ_C_MACRO, NULL, NULL, NULL);

	register_type(_bool, "bool", PQ_INT, NULL, NULL, LLVMInt1TypeInContext(ctx->llvm));
	register_type(_i8, "i8", PQ_INT, NULL, NULL, LLVMInt8TypeInContext(ctx->llvm));
	register_type(_i16, "i16", PQ_INT, NULL, NULL, LLVMInt16TypeInContext(ctx->llvm));
	register_type(_i32, "i32", PQ_INT, NULL, NULL, LLVMInt32TypeInContext(ctx->llvm));
	register_type(_i64, "i64", PQ_INT, NULL, NULL, LLVMInt64TypeInContext(ctx->llvm));
	register_type(_i128, "i128", PQ_INT, NULL, NULL, LLVMInt128TypeInContext(ctx->llvm));
	register_type(_u8, "u8", PQ_INT, NULL, NULL, LLVMInt8TypeInContext(ctx->llvm));
	register_type(_u16, "u16", PQ_INT, NULL, NULL, LLVMInt16TypeInContext(ctx->llvm));
	register_type(_u32, "u32", PQ_INT, NULL, NULL, LLVMInt32TypeInContext(ctx->llvm));
	register_type(_u64, "u64", PQ_INT, NULL, NULL, LLVMInt64TypeInContext(ctx->llvm));
	register_type(_u128, "u128", PQ_INT, NULL, NULL, LLVMInt128TypeInContext(ctx->llvm));

	register_type(_string, "string", PQ_STRING, NULL, &_free_data, LLVMPointerType(LLVMInt8TypeInContext(ctx->llvm), 0));

	register_type(_half, "half", PQ_FLOAT, NULL, NULL, LLVMHalfTypeInContext(ctx->llvm));
	register_type(_float, "float", PQ_FLOAT, NULL, NULL, LLVMFloatTypeInContext(ctx->llvm));
	register_type(_double, "double", PQ_FLOAT, NULL, NULL, LLVMDoubleTypeInContext(ctx->llvm));
#undef register_type
	return 1;
}

////////////////////////////////////////////////////////////////////////////////
//  Builtin functions
////////////////////////////////////////////////////////////////////////////////
static pq_value *_print(pq_context *ctx, int argc, pq_value **argv) {
	FILE *output = stdout;
	while(argc--) {
		pq_value *val = *(argv++);
		pq_fprint(ctx, val, output);
	}
	fputc('\n', stdout);
	return pq_return(ctx, pq_value_nil(ctx));
}
static pq_value *_let(pq_context *ctx, int argc, pq_value **argv) {
	const char *sym;
	argv[1] = pq_eval(ctx, argv[1]);
	if(!pq_is_error(argv[1])) {
		switch(argv[0]->type->kind) {
			case PQ_SYMBOL:
				sym = pq_value_get_data_as(argv[0], char *);
				pq_context_set(ctx, sym, argv[1]);
				break;

			default:
				return pq_value_ferror(ctx, "Invalid argument 1 for let: expected symbol, found %s",
						argv[0]->type->name);
		}
	}
	return argv[1];
}
/// Just return the evaluation of the evaluated argument.
static pq_value *_eval(pq_context *ctx, int argc, pq_value **argv) {
	pq_value *code = pq_eval(ctx, argv[0]);
	return pq_eval(ctx, code);
}
/// Just return argument without evaluation.
static pq_value *_quote(pq_context *ctx, int argc, pq_value **argv) {
	return argv[0];
}
/// Return NULL, so the REPL quits.
static pq_value *_quit(pq_context *ctx, int argc, pq_value **argv) {
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////

int pq_register_builtin_functions(pq_context *ctx) {
	pq_register_c_function(ctx, "let", &_let, 2, PQ_NO_VARARGS, 1);
	pq_register_c_function(ctx, "quote", &_quote, 1, PQ_NO_VARARGS, 1);
	pq_register_c_function(ctx, "eval", &_eval, 1, PQ_NO_VARARGS, 1);
	pq_register_c_function(ctx, "print", &_print, 1, PQ_VARARGS, 0);
	pq_register_c_function(ctx, "quit", &_quit, 0, PQ_NO_VARARGS, 1);
}

