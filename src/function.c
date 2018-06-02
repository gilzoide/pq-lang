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
#include <pq/context.h>

#include <stdarg.h>

pq_value *pq_register_function(pq_context *ctx, const char *name, pq_list code, uint8_t argnum, uint8_t is_variadic) {
	pq_value *func_val;
	if(func_val = pq_value_from_code(ctx, code, argnum, is_variadic)) {
		pq_context_set(ctx, name, func_val);
	}
	return func_val;
}

pq_value *pq_register_c_function(pq_context *ctx, const char *name, pq_c_function_ptr func, uint8_t argnum, enum pq_function_flags flags) {
	pq_value *func_val;
	if(func_val = pq_value_from_c_function(ctx, func, argnum, flags)) {
		pq_context_set(ctx, name, func_val);
	}
	return func_val;
}

pq_value *pq_register_native_function(pq_context *ctx, const char *name, void *fptr, pq_type *signature) {
	pq_value *func_val;
	if(func_val = pq_value_from_native_function(ctx, fptr, signature)) {
		pq_context_set(ctx, name, func_val);
	}
	return func_val;
}

pq_value *pq_register_compiler_macro(pq_context *ctx, const char *name, pq_compiler_macro_ptr macro, uint8_t argnum, enum pq_function_flags flags) {
	pq_value *func_val;
	if(func_val = pq_value_from_compiler_macro(ctx, macro, argnum, flags)) {
		pq_context_set(ctx, name, func_val);
	}
	return func_val;
}

// Evaluate the arguments given to `pq_call`, if it is a function
#define _eval_args() \
{ \
	int i; \
	for(i = 0; i < argc; i++) \
		if(pq_is_error(argv[i] = pq_eval(ctx, argv[i]))) \
			return argv[i]; \
}

pq_value *pq_call(pq_context *ctx, pq_value *func, int argc, pq_value **argv) {
	if(!func) {
		return pq_value_error(ctx, "Can't call a null value");
	}
	else if(!pq_is_callable(func)) {
		return pq_value_ferror(ctx, "Can't call a value of type \"%s\"", func->type->name);
	}
	else {
		pq_function_metadata *func_md = pq_value_get_data(func);
		enum pq_function_flags is_variadic = func_md->flags & PQ_VARIADIC;
		if(argc < (int) func_md->argnum || !is_variadic && argc > (int) func_md->argnum) {
			return pq_value_ferror(ctx, "Expected %s%u argument(s); found %d",
					is_variadic ? "at least " : "", func_md->argnum, argc);
		}

		if(func_md->flags & PQ_EVAL_ARGS) _eval_args();
		if(func_md->flags & PQ_PUSH_SCOPE) pq_push_scope(ctx);
		// TODO: create right signature for variadic calls
		switch(func->type->kind) {
			case PQ_FUNCTION:
				break;

			case PQ_C_FUNCTION: {
				pq_c_function *func_val = (pq_c_function *) func_md;
				return func_md->flags & PQ_COMPILER_MACRO
				       ? func_val->callable.macro_ptr(ctx, NULL, argc, argv)
				       : func_val->callable.function_ptr(ctx, argc, argv);
			}

			case PQ_NATIVE_FUNCTION: {
				pq_native_function *func_val = (pq_native_function *) func_md;
				void *native_argv[argc];
				int i;
				for(i = 0; i < argc; i++) {
					native_argv[i] = pq_value_get_data(argv[i]);
				}
				jit_type_t signature = jit_type_remove_tags(func_md->signature->jit_type);
				pq_value *return_value = pq_new_variable(ctx, pq_type_get_return_type(func_md->signature));
				jit_apply(signature, func_val->function_ptr, native_argv, argc, pq_value_get_data(return_value));
				return return_value;
			}

			default: break;
		}
		return pq_value_ferror(ctx, "%s type cannot be called yet (not implemented)",
				func->type->name);
	}
}
#undef _eval_args

pq_value *pq_vcall(pq_context *ctx, pq_value *func, int argc, ...) {
	pq_value *argv[argc];
	va_list ap;
	va_start(ap, argc);
	int i;
	for(i = 0; i < argc; i++) {
		argv[i] = va_arg(ap, pq_value *);
	}
	va_end(ap);
	return pq_call(ctx, func, argc, argv);
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

