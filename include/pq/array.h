/*
 * Copyright 2017-2019 Gil Barbosa Reis <gilzoide@gmail.com>
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

/** @file array.h
 * General purpose sequential array of elements with the same type.
 */

#ifndef __PQ_ARRAY_H__
#define __PQ_ARRAY_H__

#include "utils.h"

#include <stdint.h>

typedef struct pq_array {
	void *data;
	int size : SIZE_IN_BITS(int) - 1;
	int owns_data : 1;
} pq_array;

#endif


