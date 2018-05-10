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
	q->capacity = initial_capacity;
	q->size = 1;
	if((q->scopes = malloc(initial_capacity * sizeof(pq_scope))) != NULL) {
		pq_scope_initialize(q->scopes);
		return 1;
	}
	return 0;
}

void pq_scope_queue_destroy(pq_context *ctx, pq_scope_queue *q) {
	pq_scope *top;
	while(top = pq_scope_queue_pop(q)) {
		pq_scope_destroy(ctx, top);
	}
	free(q->scopes);
}

pq_scope *pq_scope_queue_peek(pq_scope_queue *q) {
	return q->size > 0 ? q->scopes + q->size - 1 : NULL;
}

pq_scope *pq_scope_queue_push(pq_scope_queue *q) {
	pq_scope *aux;
	if(q->size == q->capacity) {
		size_t new_capacity = q->capacity * 2;
		if((aux = realloc(q->scopes, new_capacity * sizeof(pq_scope))) != NULL) {
			q->scopes = aux;
			q->capacity = new_capacity;
		}
		else {
			return NULL;
		}
	}
	aux = q->scopes + (q->size++);
	pq_scope_initialize(aux);
	return aux;
}

pq_scope *pq_scope_queue_pop(pq_scope_queue *q) {
	if(q->size > 0) {
		q->size--;
		return q->scopes + q->size;
	}
	else return NULL;
}

pq_value *pq_scope_queue_get(const pq_scope_queue *q, pq_symbol sym) {
	int i;
	pq_value *val;
	for(i = q->size - 1; i >= 0; i--) {
		if((val = pq_scope_get(q->scopes + i, sym)) != NULL) {
			return val;
		}
	}
	return NULL;
}

void pq_scope_queue_set(pq_scope_queue *q, pq_symbol sym, pq_value *val) {
	pq_scope_set(q->scopes + q->size - 1, sym, val);
}

