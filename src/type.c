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

#include <pq/type.h>
#include <pq/context.h>
#include <pq/value.h>

#include <stdlib.h>
#include <assert.h>
#include <string.h>

pq_value *pq_register_type(pq_context *ctx, const char *name, pq_type_kind kind,
                           pq_type *extends, pq_destructor self_destructor,
                           pq_destructor value_destructor, void *data) {
	assert(ctx && kind < PQ_TYPE_KIND_END);
	pq_type *new_type;
	if(new_type = malloc(sizeof(pq_type))) {
		new_type->name = strdup(name);
		new_type->kind = kind;
		new_type->extends = extends;
		new_type->self_destructor = self_destructor;
		new_type->value_destructor = value_destructor;
		new_type->data = data;

		pq_value *type_val;
		if(type_val = pq_value_from_type(ctx, new_type)) {
			pq_context_set(ctx, name, type_val);
		}
		return type_val;
	}
	else {
		return NULL;
	}
}

////////////////////////////////////////////////////////////////////////////////
//  Builtin destructors
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

	register_type(_error, "error", PQ_ERROR, NULL, NULL, &destroy_error, NULL);
	register_type(_cons_cell, "cell", PQ_CONS_CELL, NULL, NULL, &destroy_cons, NULL);
	/* register_type(_scope, "scope", PQ_SCOPE, NULL, NULL,  */
	register_type(_i32, "i32", PQ_LLVM, NULL, NULL, NULL, LLVMInt32TypeInContext(ctx->llvm));
#undef register_type
}

