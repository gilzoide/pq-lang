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

#include <pq/memory_manager.h>
#include <pq/context.h>

int pq_memory_manager_initialize(pq_memory_manager *mgr) {
	return 1;
}

void pq_memory_manager_destroy(pq_context *ctx, pq_memory_manager *mgr) {}

pq_value *pq_new_value_with_size(pq_context *ctx, size_t data_size) {
	pq_value *val;
	if(val = malloc(sizeof(pq_value) + data_size)) {
		size_t last_scope = val->parent_scope = ctx->scopes.size - 1;
		pq_scope_mark_value_for_destruction(ctx->scopes.scopes + last_scope, val);
	}
	return val;
}

void pq_release_value(pq_context *ctx, pq_value *val) {
	// only destroy val if it's scope isn't yet below in the stack
	if(val && val->parent_scope >= ctx->scopes.size - 1) {
		if(val->type->value_destructor) {
			val->type->value_destructor(ctx, pq_value_get_data(val));
		}
		free(val);
	}
}

