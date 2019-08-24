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

#include "core.h"

#include <pq/context.h>
#include <pq/value.h>

void pq_list_fprint(pq_context *ctx, pq_list lst, FILE *output) {
	int i;
	fputc('(', output);
	for(i = 0; i < lst.size; i++) {
		pq_fprint(ctx, lst.values[i], output);
		if(i < lst.size - 1) {
			fputc(' ', output);
		}
	}
	fputc(')', output);
}
void pq_tuple_fprint(pq_context *ctx, pq_list lst, FILE *output) {
	int i;
	fputc('{', output);
	for(i = 0; i < lst.size; i++) {
		pq_fprint(ctx, lst.values[i], output);
		if(i < lst.size - 1) {
			fputc(' ', output);
		}
	}
	fputc('}', output);
}
void pq_type_fprint(pq_context *ctx, pq_type *type, FILE *output) {
	if(type->name) {
		fputs(type->name, output);
	}
	else {
		switch(type->kind) {
			case PQ_SIGNATURE: {
				   int i, num_args = pq_type_get_num_arguments(type);
				   pq_type *ret_type = pq_type_get_return_type(type);
				   pq_type **arg_types = pq_type_get_argument_types(type);
				   int is_variadic = pq_type_get_is_variadic(type);
				   fputc('[', output);
				   pq_type_fprint(ctx, ret_type, output);
				   for(i = 0; i < num_args; i++) {
					   fputc(' ', output);
					   pq_type_fprint(ctx, arg_types[i], output);
				   }
				   if(is_variadic) {
					   fputs(" ...", output);
				   }
				   fputc(']', output);
				}
				break;

			case PQ_POINTER:
				fprintf(output, "(pointer ");
				pq_type_fprint(ctx, pq_type_get_underlying_type(type), output);
				fputc(')', output);
				break;

			default: fputs("[no print for type yet]", output); break;
		}
	}
}
void pq_function_fprint(pq_context *ctx, pq_function *func, FILE *output) {
	fprintf(output, "(function ");
	if(func->header.symbol != PQ_SYMBOL_NIL) {
		fprintf(output, "'%s ", pq_string_from_symbol(ctx, func->header.symbol));
	}
	pq_list_fprint(ctx, func->args, output);
	fputs("          ", output);
	int i;
	for(i = 0; i < func->code.size; i++) {
		fputc(' ', output);
		pq_fprint(ctx, func->code.values[i], output);
	}
	fputc(')', output);
}
void pq_c_function_fprint(pq_context *ctx, pq_c_function *func, FILE *output) {
	fprintf(output, "(%s ", func->header.flags & PQ_COMPILER_MACRO ? "compiler-macro" : "c-function");
	if(func->header.symbol != PQ_SYMBOL_NIL) {
		fprintf(output, "'%s ", pq_string_from_symbol(ctx, func->header.symbol));
	}
	if(func->header.signature) {
		pq_type_fprint(ctx, func->header.signature, output);
	    fputc(' ', output);
	}
	fprintf(output, "%p)", func->callable.function_ptr);
}
void pq_overload_fprint(pq_context *ctx, pq_overload *overload, FILE *output) {
	pq_overload_iterator it = pq_overload_new_iterator(overload);
	pq_value *val;
	fprintf(output, "<Overload ");
	if(overload->symbol != PQ_SYMBOL_NIL) {
		fprintf(output, pq_string_from_symbol(ctx, overload->symbol));
	}
	while(val = pq_overload_next_function(overload, &it)) {
		fputs("\n ", output);
		pq_function_metadata *func_md = pq_value_get_data(val);
		pq_type *signature = func_md->signature;
		if(signature == NULL) {
			fprintf(output, "*");
		}
		else {
			pq_type_fprint(ctx, signature, output);
		}
	}
	fputc('>', output);
}
void pq_fprint(pq_context *ctx, pq_value *val, FILE *output) {
	switch(val->type->kind) {
		case PQ_INT:
			fprintf(output, "%ld", pq_value_as_int(val));
			break;

		case PQ_FLOAT:
			fprintf(output, "%g", pq_value_as_double(val));
			break;

		case PQ_STRING:
			fprintf(output, "%s", pq_value_get_data_as(val, char *));
			break;

		case PQ_NIL:
			fputs("nil", output);
			break;

		case PQ_BOOL:
			fputs(pq_value_as_bool(val) ? "true" : "false", output);
			break;

		case PQ_SYMBOL:
			fprintf(output, "'%s", pq_string_from_symbol(ctx, pq_value_get_data_as(val, pq_symbol)));
			break;

		case PQ_LIST:
			pq_list_fprint(ctx, pq_value_get_data_as(val, pq_list), output);
			break;

		case PQ_ERROR:
			fprintf(output, "Error: %s", pq_value_get_data_as(val, char *));
			break;

		case PQ_TYPE:
			pq_type_fprint(ctx, pq_value_get_data_as(val, pq_type *), output);
			break;

		case PQ_FUNCTION:
			pq_function_fprint(ctx, pq_value_get_data(val), output);
			break;

		case PQ_C_FUNCTION:
			pq_c_function_fprint(ctx, pq_value_get_data(val), output);
			break;

		case PQ_OVERLOAD:
			pq_overload_fprint(ctx, pq_value_get_data(val), output);
			break;

		case PQ_POINTER:
			fprintf(output, "%p", pq_value_get_data_as(val, void *));
			break;

        case PQ_TUPLE:
            pq_tuple_fprint(ctx, pq_value_get_data_as(val, pq_list), output);
            break;

		default:
			fprintf(output, "[no print for type \"%s\"]", val->type->name);
			break;
	}
}

void pq_print(pq_context *ctx, pq_value *val) {
	pq_fprint(ctx, val, stdout);
}

/// Print all values given.
static pq_value *_print(pq_context *ctx, int argc, pq_value **argv) {
	FILE *output = stdout; // TODO: use environment
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
int pq_register_core_print(pq_context *ctx) {
	return pq_register_c_function(ctx, "print", &_print, 1, PQ_VARIADIC | PQ_EVAL_ARGS) !=  NULL;
}

