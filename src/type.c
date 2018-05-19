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
		new_type->jit_type = jit_type;
		new_type->value_destructor = value_destructor;
	}
	return new_type;
}

void pq_type_destroy(pq_type *type) {
	jit_type_free(type->jit_type);
	free(type->name);
	free(type);
}

