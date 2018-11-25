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

/** @file memory_manager.h
 * Memory manager for all Values used by a pq context.
 *
 * This is C, memory is important.
 */

#ifndef __PQ_MEMORY_MANAGER_H__
#define __PQ_MEMORY_MANAGER_H__

#include "array.h"
#include "list.h"
#include "value.h"

#include <stdlib.h>

/**
 * Pq internal memory manager.
 */
typedef struct pq_memory_manager {
} pq_memory_manager;

// Forward declarations
typedef struct pq_context pq_context;

/**
 * Initialize a pq Memory Manager.
 */
int pq_memory_manager_initialize(pq_memory_manager *mgr);
/**
 * Destroy a Memory Manager, freeing the memory used.
 */
void pq_memory_manager_destroy(pq_context *ctx, pq_memory_manager *mgr);


/**
 * Acquire a new Value, with additional `data_size` bytes for the custom data.
 *
 * This is important if Values memory management change over time (maybe a
 * memory pool, someday).
 *
 * @sa pq_new_value
 */
pq_value *pq_new_value_with_size(pq_context *ctx, size_t data_size);
/// Facility to create a Value with the `sizeof` a C type.
#define pq_new_value(ctx, c_type) (pq_new_value_with_size(ctx, sizeof(c_type)))
/**
 * Release a Value.
 *
 * This is important if Values memory management change over time (maybe a
 * memory pool, someday).
 */
void pq_release_value(pq_context *ctx, pq_value *val);

/**
 * Acquire a new List with the specified size.
 */
pq_list pq_new_list_with_size(pq_context *ctx, int size);
/**
 * Release a List underlying value list.
 */
void pq_release_list(pq_context *ctx, pq_list *lst);

/**
 * Acquire a new Array with the element type and size.
 */
pq_array pq_new_array_with_size(pq_context *ctx, int size, pq_type *type);
/**
 * Release an Array underlying data.
 */
void pq_release_array(pq_context *ctx, pq_array *arr);

#endif

