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

/** @file list.h
 * List of values, used as code and as a general purpose list.
 */

#ifndef __PQ_LIST_H__
#define __PQ_LIST_H__

#include "utils.h"

// Forward declaration
typedef struct pq_value pq_value;

/**
 * General purpose heterogenous list of values.
 */
typedef struct pq_list {
	pq_value **values;
	int size : SIZE_IN_BITS(int) - 1;
	int owns_data : 1;
} pq_list;

/**
 * Get the Value at the specified index of the List.
 *
 * Negative indexes refer to items in the end of the List.
 *
 * @return Value at index.
 * @return `NULL` if `index` is out of list bounds.
 */
pq_value *pq_list_value_at(pq_list lst, int index);

/**
 * Creates a slice of `lst` with the items starting in `from` until `to`
 * indexes.
 *
 * Negative indexes refer to items in the end of the List.
 *
 * @warning This doesn't allocate memory for the internal array of the returned
 *  slice.
 *
 * @param lst  Original list.
 * @param from Starting index.
 * @param to   End index.
 *
 * @return List slice.
 */
pq_list pq_list_slice(pq_list lst, int from, int to);

#endif

