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

/** @file utils.h
 * Internal utilities.
 */

#ifndef __PQ_UTILS_H__
#define __PQ_UTILS_H__

#include <assert.h>
#include <stdlib.h>

/// Default internal API error generator
#define PQ_API_ERROR(ctx, fmt, ...) \
	pq_value_ferror(ctx, "[%s] " fmt, __func__, ##__VA_ARGS__)

/// API assert wrapper
#define PQ_ASSERT(cond, msg) \
	assert((cond) && (msg))

/// Number of elements in a static sized array
#define ARRAY_SIZE(a) \
	(sizeof(a) / sizeof(a[0]))

#define SIZE_IN_BITS(c_type) \
	(8 * sizeof(c_type))

static inline int max_int(int a, int b) {
	return a > b ? a : b;
}
static inline int min_int(int a, int b) {
	return a < b ? a : b;
}
#define CLAMP_INT(value, min, max) \
	(min_int(max_int((value), (min)), (max)))

/**
 * Dynamic list with constant growth rate.
 */
typedef struct pq_vector {
	void *arr;
	size_t size;
	size_t capacity;
} pq_vector;

#define PQ_POINTER_VECTOR_GROWTH_RATE 1.5

int pq_vector_initialize(pq_vector *vec, size_t initial_capacity, size_t member_size);
#define pq_vector_initialize_as(vec, initial_capacity, type) \
	(pq_vector_initialize(vec, initial_capacity, sizeof(type)))
void pq_vector_destroy(pq_vector *vec);
void *pq_vector_push(pq_vector *vec, size_t member_size);
#define pq_vector_push_as(vec, type) \
	((type *) pq_vector_push(vec, sizeof(type)))
void *pq_vector_pop(pq_vector *vec, size_t member_size);
#define pq_vector_pop_as(vec, type) \
	((type *) pq_vector_pop(vec, sizeof(type)))
void *pq_vector_peek(const pq_vector *vec, size_t member_size);
#define pq_vector_peek_as(vec, type) \
	((type *) pq_vector_peek(vec, sizeof(type)))
#define pq_vector_at(vec, i, type) \
	(((type *) (vec)->arr) + i)

#endif

