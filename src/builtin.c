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
		new_val->type = ctx->type_manager._nil;
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
static void _free_data(pq_context *ctx, void *data) {
	free(*((void **) data));
}

int pq_register_builtin_types(pq_context *ctx) {
	pq_type *type;
	pq_value *type_val;
	if(type = pq_register_type(ctx, "type", PQ_TYPE, NULL, NULL)) {
		ctx->type_manager._type = type;
		if(type_val = pq_value_from_type(ctx, type)) {
			pq_context_set(ctx, "type", type_val);
		}
		else return 0;
	}
	else return 0;

#define register_type(builtin_var, name, kind, jit_type, val_dtor) \
	if((type = pq_register_type(ctx, name, kind, jit_type, val_dtor)) && (type_val = pq_value_from_type(ctx, type))) { \
		ctx->type_manager. builtin_var = type; \
		pq_context_set(ctx, name, type_val); \
	} \
	else return 0

	register_type(_error, "error-t", PQ_ERROR, NULL, &_free_data);
	register_type(_list, "list-t", PQ_LIST, NULL, (pq_destructor) &pq_release_list);
	register_type(_scope, "scope-t", PQ_SCOPE, NULL, (pq_destructor) &pq_scope_destroy);
	register_type(_nil, "nil-t", PQ_NIL, NULL, NULL);
	register_type(_symbol, "symbol-t", PQ_SYMBOL, jit_type_void_ptr, NULL);

	register_type(_bool, "bool", PQ_INT, jit_type_sys_bool, NULL);
	register_type(_i8,   "i8",   PQ_INT, jit_type_sbyte,    NULL);
	register_type(_i16,  "i16",  PQ_INT, jit_type_short,    NULL);
	register_type(_i32,  "i32",  PQ_INT, jit_type_int,      NULL);
	register_type(_i64,  "i64",  PQ_INT, jit_type_long,     NULL);
	register_type(_u8,   "u8",   PQ_INT, jit_type_ubyte,    NULL);
	register_type(_u16,  "u16",  PQ_INT, jit_type_ushort,   NULL);
	register_type(_u32,  "u32",  PQ_INT, jit_type_uint,     NULL);
	register_type(_u64,  "u64",  PQ_INT, jit_type_ulong,    NULL);

	register_type(_float,  "float",  PQ_FLOAT, jit_type_float32, NULL);
	register_type(_double, "double", PQ_FLOAT, jit_type_float64, NULL);

	register_type(_string, "string", PQ_STRING, NULL, &_free_data);

	register_type(_pointer, "pointer", PQ_POINTER, jit_type_void_ptr, NULL);

	register_type(_function,   "function-t",   PQ_FUNCTION,   NULL, NULL);
	register_type(_c_function, "c-function-t", PQ_C_FUNCTION, NULL, NULL);
#undef register_type
	return 1;
}

////////////////////////////////////////////////////////////////////////////////
//  Builtin functions
////////////////////////////////////////////////////////////////////////////////

/// Print all values given.
static pq_value *_print(pq_context *ctx, int argc, pq_value **argv) {
	FILE *output = stdout;
	const char *sep = " ";
	const char *end = "\n";
	int i;
	for(i = 0; i < argc; i++) {
		pq_fprint(ctx, argv[i], output);
		fputs(sep, output);
	}
	fputs(end, output);
	return pq_value_nil(ctx);
}
static pq_value *_let(pq_context *ctx, int argc, pq_value **argv) {
	pq_symbol sym;
	argv[1] = pq_eval(ctx, argv[1]);
	pq_assert_not_error(argv[1]);
	switch(argv[0]->type->kind) {
		case PQ_SYMBOL:
			sym = pq_value_get_data_as(argv[0], pq_symbol);
			pq_context_set_symbol(ctx, sym, argv[1]);
			break;

		default:
			return pq_value_ferror(ctx, "Invalid argument 1: expected symbol, found %s",
					argv[0]->type->name);
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
/// Get the Type of a Value.
static pq_value *_type_of(pq_context *ctx, int argc, pq_value **argv) {
	return pq_value_from_type(ctx, argv[0]->type);
}
////////////////////////////////////////////////////////////////////////////////

int pq_register_builtin_functions(pq_context *ctx) {
	pq_register_c_function(ctx, "let", &_let, 2, 0);
	pq_register_c_function(ctx, "quote", &_quote, 1, 0);
	pq_register_c_function(ctx, "eval", &_eval, 1, 0);
	pq_register_c_function(ctx, "print", &_print, 1, PQ_VARIADIC | PQ_EVAL_ARGS);
	pq_register_c_function(ctx, "quit", &_quit, 0, 0);
	pq_register_c_function(ctx, "type-of", &_type_of, 1, PQ_EVAL_ARGS);
	return 1;
}

