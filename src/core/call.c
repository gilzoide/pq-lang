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
#include <pq/value.h>

#include <stdarg.h>

static inline pq_value *_pq_call_function(pq_context *ctx, pq_value *func, int is_variadic, pq_value **argv) {
	int i;
	pq_function f = pq_value_get_data_as(func, pq_function);
	for(i = 0; i < f.args.size - is_variadic; i++) {
		pq_context_set_symbol(ctx, pq_value_as_symbol(f.args.values[i]), argv[i]);
	}
	if(is_variadic) {
		pq_context_set_symbol(ctx, pq_value_as_symbol(f.args.values[i]), pq_value_list_from_values(ctx, argv + i, f.args.size - i));
	}
	pq_value *val;
	for(i = 0; i < f.code.size; i++) {
		if(pq_is_error(val = pq_eval(ctx, f.code.values[i]))) break;
	}
	return val;
}

static inline pq_value *_pq_call_c_function(pq_context *ctx, pq_function_metadata *func_md, int argc, pq_value **argv) {
	pq_c_function *func_val = (pq_c_function *)func_md;
	return func_md->flags & PQ_COMPILER_MACRO
			? func_val->callable.macro_ptr(ctx, NULL, argc, argv)
			: func_val->callable.function_ptr(ctx, argc, argv);
}

static inline pq_value *_pq_call_native_function(pq_context *ctx, pq_function_metadata *func_md, int argc, pq_value **argv) {
	pq_native_function *func_val = (pq_native_function *)func_md;
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

static inline pq_value *_pq_find_overloaded_function_for_call(pq_context *ctx, pq_overload *overload, int argc, pq_value **argv) {
	pq_type *arg_types[argc];
	int i;
	for(i = 0; i < argc; i++) {
		arg_types[i] = argv[i]->type;
	}
	return pq_overload_for_types(ctx, overload, argc, arg_types);
}

static inline pq_value *_pq_eval_arguments(pq_context *ctx, int argc, pq_value **argv, pq_value **evaluated) {
	int i;
	for(i = 0; i < argc; i++) {
		if(pq_is_error(evaluated[i] = pq_eval(ctx, argv[i]))) return evaluated[i];
	}
	return NULL;
}
pq_value *pq_call(pq_context *ctx, pq_value *func, int argc, pq_value **argv) {
	if(!func) {
		return pq_value_error(ctx, "Can't call a null value");
	}
	else if(!pq_is_callable(func)) {
		return pq_value_ferror(ctx, "Can't call a value of type \"%s\"", func->type->name);
	}
	else {
		pq_value *evaluated[argc], *evaluation_error;
		int is_overload = pq_is_overload(func);
		if(is_overload) {
			if((evaluation_error = _pq_eval_arguments(ctx, argc, argv, evaluated)) != NULL) return evaluation_error;
			argv = evaluated;
			pq_value *function_for_call = _pq_find_overloaded_function_for_call(ctx, (pq_overload *)pq_value_get_data(func), argc, argv);
			if(pq_is_nil(function_for_call)) {
				return PQ_API_ERROR(ctx, "No overload found");
			}
			func = function_for_call;
		}

		pq_function_metadata *func_md = pq_value_get_data(func);
		enum pq_function_flags is_variadic = func_md->flags & PQ_VARIADIC;
		if(argc < (int)func_md->argnum || !is_variadic && argc > (int)func_md->argnum) {
			return pq_value_ferror(ctx, "Expected %s%u argument(s); found %d",
					is_variadic ? "at least " : "", func_md->argnum, argc);
		}

		if(!is_overload && func_md->flags & PQ_EVAL_ARGS) {
			if((evaluation_error = _pq_eval_arguments(ctx, argc, argv, evaluated)) != NULL) return evaluation_error;
			argv = evaluated;
		}
		if(func_md->flags & PQ_PUSH_SCOPE) pq_push_scope(ctx);
		// TODO: create right signature for variadic calls
		pq_value *return_value;
		switch(func->type->kind) {
			case PQ_FUNCTION:
				return_value = _pq_call_function(ctx, func, is_variadic, argv);
				break;

			case PQ_C_FUNCTION:
				return_value = _pq_call_c_function(ctx, func_md, argc, argv);
				break;

			case PQ_NATIVE_FUNCTION:
				return_value = _pq_call_native_function(ctx, func_md, argc, argv);
				break;

			default:
				return_value = NULL;
				break;
		}
		if(return_value == NULL) return_value = pq_value_ferror(ctx, "%s type cannot be called yet (not implemented)", func->type->name);
		return func_md->flags & PQ_PUSH_SCOPE ? pq_return(ctx, return_value) : return_value;
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

