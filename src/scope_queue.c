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

#include <pq/scope_queue.h>

#define PQ_DEFAULT_INITIAL_SCOPE_QUEUE_CAPACITY 4

int pq_scope_queue_initialize(pq_scope_queue *q, size_t initial_capacity) {
	if(initial_capacity == 0) {
		initial_capacity = PQ_DEFAULT_INITIAL_SCOPE_QUEUE_CAPACITY;
	}
	return pq_vector_initialize_as(&q->scopes, initial_capacity, pq_scope)
	       && pq_scope_initialize(pq_vector_push_as(&q->scopes, pq_scope));
}

void pq_scope_queue_destroy(pq_context *ctx, pq_scope_queue *q) {
	pq_scope *top;
	while(top = pq_vector_pop_as(&q->scopes, pq_scope)) {
		pq_scope_destroy(ctx, top);
	}
	pq_vector_destroy(&q->scopes);
}

pq_scope *pq_scope_queue_peek(pq_scope_queue *q) {
	return pq_vector_peek_as(&q->scopes, pq_scope);
}

pq_scope *pq_scope_queue_push(pq_scope_queue *q) {
	pq_scope *aux;
	if(aux = pq_vector_push_as(&q->scopes, pq_scope)) {
		pq_scope_initialize(aux);
	}
	return aux;
}

pq_scope *pq_scope_queue_pop(pq_scope_queue *q) {
	return pq_vector_pop_as(&q->scopes, pq_scope);
}

pq_value *pq_scope_queue_get(const pq_scope_queue *q, pq_symbol sym) {
	int i;
	pq_value *val;
	for(i = q->scopes.size - 1; i >= 0; i--) {
		if(val = pq_scope_get(pq_vector_at(&q->scopes, i, pq_scope), sym)) {
			return val;
		}
	}
	return NULL;
}

pq_value *pq_scope_queue_get_from_top(const pq_scope_queue *q, pq_symbol sym) {
	return pq_scope_get(pq_vector_peek_as(&q->scopes, pq_scope), sym);
}

pq_value *pq_scope_queue_get_from_bottom(const pq_scope_queue *q, pq_symbol sym) {
	return pq_scope_get(pq_vector_at(&q->scopes, 0, pq_scope), sym);
}

void pq_scope_queue_set(pq_scope_queue *q, pq_symbol sym, pq_value *val) {
	pq_scope_set(pq_vector_peek_as(&q->scopes, pq_scope), sym, val);
}

void pq_scope_queue_set_global(pq_scope_queue *q, pq_symbol sym, pq_value *val) {
	pq_scope_set(pq_vector_at(&q->scopes, 0, pq_scope), sym, val);
}

