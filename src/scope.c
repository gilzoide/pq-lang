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

#include <pq/scope.h>
#include <pq/memory_manager.h>

#define PQ_SCOPE_INITIAL_CAPACITY 4

int pq_scope_initialize(pq_scope *scope) {
	scope->table = NULL;
	scope->capacity = PQ_SCOPE_INITIAL_CAPACITY;
	if(scope->created_values = malloc(PQ_SCOPE_INITIAL_CAPACITY * sizeof(pq_value *))) {
		scope->size = 0;
		return 1;
	}
	return 0;
}

void pq_scope_destroy(pq_context *ctx, pq_scope *scope) {
	Word_t bytes;
	JSLFA(bytes, scope->table);
	// Destroy values created at this Scope
	// The reverse order is important, as newer Values may reference older ones:
	// this is particularly true for builtin Types.
	int i;
	for(i = scope->size - 1; i >= 0; i--) {
		pq_context_release_value(ctx, scope->created_values[i]);
	}
	free(scope->created_values);
}

pq_value *pq_scope_get(const pq_scope *scope, const char *key) {
	Word_t *pvalue;
	JSLG(pvalue, scope->table, key);
	return pvalue ? (pq_value *) *pvalue : NULL;
}

void pq_scope_set(pq_scope *scope, const char *key, pq_value *val) {
	Word_t *pvalue;
	JSLI(pvalue, scope->table, key);
	*pvalue = (Word_t) val;
}

int pq_scope_mark_value_for_destruction(pq_scope *scope, pq_value *val) {
	pq_value **aux;
	if(scope->size == scope->capacity) {
		size_t new_capacity = scope->capacity * 2;
		if((aux = realloc(scope->created_values, new_capacity * sizeof(pq_value *))) != NULL) {
			scope->created_values = aux;
			scope->capacity = new_capacity;
		}
		else {
			return 0;
		}
	}
	scope->created_values[scope->size++] = val;
	return 1;
}

