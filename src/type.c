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

#include <pq/type.h>
#include <pq/context.h>
#include <pq/value.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define PQ_TYPE_METADATA_TAG_KIND 1

pq_type *pq_create_type(const char *name, enum pq_type_kind kind,
                         jit_type_t jit_type, pq_destructor value_destructor) {
	assert(kind >= PQ_INT && kind < PQ_TYPE_KIND_END && "Invalid type kind enum value");
	pq_type *new_type;
	if(new_type = malloc(sizeof(pq_type))) {
		new_type->name = name ? strdup(name) : NULL;
		new_type->kind = kind;
		new_type->jit_type = jit_type_create_tagged(jit_type, PQ_TYPE_METADATA_TAG_KIND, new_type, NULL, 0);
		new_type->value_destructor = value_destructor;
	}
	return new_type;
}

pq_type *pq_create_aggregate_type(const char *name, enum pq_type_kind kind, jit_type_t jit_type,
                                  pq_type *main_subtype, unsigned int num_subtypes, pq_type **subtypes) {
	assert(kind >= PQ_TYPE_FIRST_AGGREGATE && kind <= PQ_TYPE_LAST_AGGREGATE && "Invalid type kind enum value for aggregate type");
	pq_aggregate_type *new_aggregate_type;
	if(new_aggregate_type = malloc(sizeof(pq_aggregate_type) + (num_subtypes * sizeof(pq_type *)))) {
		pq_type *new_type = &new_aggregate_type->type;
		new_type->name = name ? strdup(name) : NULL;
		new_type->kind = kind;
		new_type->jit_type = jit_type_create_tagged(jit_type, PQ_TYPE_METADATA_TAG_KIND, new_type, NULL, 0);
		new_type->value_destructor = NULL;

		new_aggregate_type->main_subtype = main_subtype;
		new_aggregate_type->num_subtypes = num_subtypes;
		memcpy(new_aggregate_type->subtypes, subtypes, num_subtypes * sizeof(pq_type *));
	}
	return (pq_type *) new_aggregate_type;
}

void pq_type_destroy(pq_type *type) {
	jit_type_free(type->jit_type);
	free(type->name);
	free(type);
}

size_t pq_type_get_value_size(pq_type *type) {
	return jit_type_get_size(type->jit_type);
}

pq_type *pq_type_from_jit(jit_type_t jit_type) {
	return jit_type_get_tagged_data(jit_type);
}

pq_type *pq_type_get_return_type(pq_type *signature) {
	return signature->kind == PQ_SIGNATURE
	       ? ((pq_aggregate_type *) signature)->main_subtype
	       : NULL;
}

int pq_type_get_num_arguments(pq_type *signature) {
	return signature->kind == PQ_SIGNATURE
	       ? ((pq_aggregate_type *) signature)->num_subtypes
	       : -1;
}

pq_type **pq_type_get_argument_types(pq_type *signature) {
	return signature->kind == PQ_SIGNATURE
	       ? ((pq_aggregate_type *) signature)->subtypes
	       : NULL;
}


int pq_type_get_num_fields(pq_type *aggregate) {
	return aggregate->kind == PQ_TUPLE || aggregate->kind == PQ_STRUCT
	       ? ((pq_aggregate_type *) aggregate)->num_subtypes
	       : -1;
}

pq_type **pq_type_get_field_types(pq_type *aggregate) {
	return aggregate->kind == PQ_TUPLE || aggregate->kind == PQ_STRUCT
	       ? ((pq_aggregate_type *) aggregate)->subtypes
	       : NULL;
}

pq_type *pq_type_get_underlying_type(pq_type *type) {
	return type->kind == PQ_ARRAY || type->kind == PQ_POINTER
	       ? ((pq_aggregate_type *) type)->main_subtype
	       : NULL;
}

