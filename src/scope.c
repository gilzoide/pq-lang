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

#include <Judy.h>

#include <pq/scope.h>
#include <pq/memory_manager.h>

#define PQ_SCOPE_INITIAL_CAPACITY 4

int pq_scope_initialize(pq_scope *scope) {
	scope->table = NULL;
	return pq_vector_initialize_as(&scope->created_values, PQ_SCOPE_INITIAL_CAPACITY, pq_value *);
}

void pq_scope_destroy(pq_context *ctx, pq_scope *scope) {
	Word_t bytes;
	JLFA(bytes, scope->table);
	// Destroy values created at this Scope
	// The reverse order is important, as newer Values may reference older ones:
	// this is particularly true for builtin Types.
	int i;
	for(i = scope->created_values.size - 1; i >= 0; i--) {
		pq_release_value(ctx, *pq_vector_at(&scope->created_values, i, pq_value *));
	}
	pq_vector_destroy(&scope->created_values);
}

pq_value *pq_scope_get(const pq_scope *scope, pq_symbol sym) {
	Word_t *pvalue;
	JLG(pvalue, scope->table, sym);
	return pvalue != NULL ? (pq_value *) *pvalue : NULL;
}

void pq_scope_set(pq_scope *scope, pq_symbol sym, pq_value *val) {
	Word_t *pvalue;
	JLI(pvalue, scope->table, sym);
	if(pvalue != PJERR) {
		*pvalue = (Word_t) val;
	}
}

int pq_scope_mark_value_for_destruction(pq_scope *scope, pq_value *val) {
	pq_value **val_ptr;
	if(val_ptr = pq_vector_push_as(&scope->created_values, pq_value *)) {
		*val_ptr = val;
	}
	return val_ptr != NULL;
}

