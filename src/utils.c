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

#include <pq/utils.h>


int pq_vector_initialize(pq_vector *vec, size_t initial_capacity, size_t member_size) {
	vec->size = 0;
	if(vec->arr = malloc(initial_capacity * member_size)) {
		vec->capacity = initial_capacity;
	}
	return vec->arr != NULL;
}

void pq_vector_destroy(pq_vector *vec) {
	free(vec->arr);
}

void *pq_vector_push(pq_vector *vec, size_t member_size) {
	if(vec->size == vec->capacity) {
		size_t new_capacity = vec->capacity * PQ_POINTER_VECTOR_GROWTH_RATE;
		void *arr;
		if(arr = realloc(vec->arr, new_capacity * member_size)) {
			vec->capacity = new_capacity;
			vec->arr = arr;
		}
		else {
			return NULL;
		}
	}
	return vec->arr + (member_size * vec->size++);
}

void *pq_vector_pop(pq_vector *vec, size_t member_size) {
	if(vec->size > 0) {
		vec->size--;
		return vec->arr + (member_size * vec->size);
	}
	else {
		return NULL;
	}
}

void *pq_vector_peek(pq_vector *vec, size_t member_size) {
	return vec->size > 0 ? vec->arr + (member_size * (vec->size - 1)) : NULL;
}

