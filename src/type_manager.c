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

#include <pq/type_manager.h>
#include <pq/context.h>

#include <assert.h>

int pq_type_manager_initialize(pq_type_manager *type_manager) {
	type_manager->tuple_table = NULL;
	type_manager->signature_table = NULL;
	return pq_vector_initialize_as(&type_manager->all_types, 32, pq_type *);
}

void pq_type_manager_destroy(pq_type_manager *type_manager) {
	Word_t rc_word;
	JHSFA(rc_word, type_manager->tuple_table);
	JHSFA(rc_word, type_manager->signature_table);

	int i;
	for(i = 0; i < type_manager->all_types.size; i++) {
		pq_type_destroy(*pq_vector_at(&type_manager->all_types, i, pq_type *));
	}
	pq_vector_destroy(&type_manager->all_types);
}

pq_type *pq_get_builtin_type(pq_context *ctx, enum pq_builtin_type builtin_type) {
	assert(builtin_type >= PQ_TYPE_TYPE && builtin_type < PQ_BUILTIN_TYPE_MAX && "Invalid builtin type enum value");
	return *pq_vector_at(&ctx->type_manager.all_types, builtin_type, pq_type *);
}

pq_type *pq_register_type(pq_context *ctx, const char *name, enum pq_type_kind kind,
                          jit_type_t jit_type, pq_destructor value_destructor) {
	pq_type *new_type;
	if(new_type = pq_create_type(name, kind, jit_type, value_destructor)) {
		pq_type **new_type_ptr;
		if(new_type_ptr = pq_vector_push_as(&ctx->type_manager.all_types, pq_type *)) {
			*new_type_ptr = new_type;
		}
		else {
			pq_type_destroy(new_type);
			return NULL;
		}
	}
	return new_type;
}

pq_type *pq_get_tuple_type(pq_context *ctx, pq_type **types, size_t n) {
	Word_t *pvalue;
	JHSI(pvalue, ctx->type_manager.tuple_table, types, n * sizeof(pq_type *));
	if(pvalue != PJERR) {
		if(*pvalue == 0) {
			jit_type_t field_types[n];
			int i;
			for(i = 0; i < n; i++) {
				field_types[i] = types[i]->jit_type;
			}
			jit_type_t tuple_jit_type = jit_type_create_struct(field_types, n, 1);
			*pvalue = (Word_t) pq_register_type(ctx, NULL, PQ_TUPLE, tuple_jit_type, NULL);
		}
		return (pq_type *) *pvalue;
	}
	else {
		return NULL;
	}
}

