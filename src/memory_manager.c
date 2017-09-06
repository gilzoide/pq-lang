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

#include <Judy.h>

#include <pq/memory_manager.h>
#include <pq/context.h>

#include <stdlib.h>

int pq_memory_manager_initialize(pq_memory_manager *mgr) {
	return 1;
}

void pq_memory_manager_destroy(pq_context *ctx, pq_memory_manager *mgr) {}

pq_value *pq_context_new_value(pq_context *ctx) {
	pq_value *val;
	if(val = malloc(sizeof(pq_value))) {
		size_t last_scope = val->parent_scope = ctx->scopes.size - 1;
		pq_scope_mark_value_for_destruction(ctx->scopes.scopes + last_scope, val);
	}
	return val;
}

void pq_context_release_value(pq_context *ctx, pq_value *val) {
	// only destroy val if it's scope isn't yet below in the stack
	if(val && val->parent_scope >= ctx->scopes.size - 1) {
		if(val->type->value_destructor) {
			val->type->value_destructor(ctx, val->data);
		}
		free(val);
	}
}

pq_cons_cell *pq_context_new_cons_cell(pq_context *ctx) {
	return malloc(sizeof(pq_cons_cell));
}

void pq_context_release_cons_cell(pq_context *ctx, pq_cons_cell *val) {
	free(val);
}

