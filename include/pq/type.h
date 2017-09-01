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

#ifndef __PQ_TYPE_H__
#define __PQ_TYPE_H__

typedef enum {
	PQ_NIL,
	PQ_LLVM,
	PQ_LIST,
	PQ_TYPE,
	PQ_SCOPE,
	PQ_SYMBOL,
	PQ_FUNCTION,
	PQ_MACRO,
	PQ_C_FUNCTION,
	PQ_LLVM_MACRO,
} pq_type_kind;

/**
 * Function that destroys a value.
 *
 * Any type may specify a custom destructor.
 */
typedef void (*pq_value_destructor)(void *);

/**
 * Pq Type: metadata about a Value's type.
 */
typedef struct {
	pq_type_kind kind;
	void *data;
	pq_value_destructor destructor;
} pq_type;

#endif

