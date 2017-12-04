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
                           pq_destructor self_destructor,
                           pq_destructor value_destructor, void *data) {
	assert(ctx && kind < PQ_TYPE_KIND_END);
	pq_type *new_type;
	if(new_type = malloc(sizeof(pq_type))) {
		new_type->name = strdup(name);
		new_type->kind = kind;
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

