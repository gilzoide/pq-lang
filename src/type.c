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

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define PQ_TYPE_METADATA_TAG_KIND 1

pq_type pq_register_type(pq_context *ctx, const char *name, pq_type_kind kind,
                         jit_type_t jit_type, pq_destructor value_destructor) {
	assert(kind >= PQ_INT && kind < PQ_TYPE_KIND_END && "Invalid type kind enum value");
	pq_type_metadata *new_type_metadata;
	if(new_type_metadata = malloc(sizeof(pq_type_metadata))) {
		new_type_metadata->name = name ? strdup(name) : NULL;
		new_type_metadata->kind = kind;
		new_type_metadata->value_destructor = value_destructor;
	}
	return new_type_metadata
			? jit_type_create_tagged(jit_type, PQ_TYPE_METADATA_TAG_KIND,
					new_type_metadata, (void (*)(void *)) &pq_type_metadata_destroy, 1)
			: NULL;
}

pq_type_metadata *pq_type_get_metadata(pq_type type) {
	return jit_type_get_tagged_data(type);
}

void pq_type_metadata_destroy(pq_type_metadata *type_metadata) {
	free(type_metadata->name);
	free(type_metadata);
}

void pq_type_destroy(pq_context *ctx, pq_type type) {
	jit_type_free(type);
}

