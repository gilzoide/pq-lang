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

/** @file builtin.h
 * Pq builtin Types, Values, Functions and Macros.
 */

#ifndef __PQ_BUILTIN_H__
#define __PQ_BUILTIN_H__

#include "function.h"
#include "type.h"
#include "value.h"

// Forward declarations
typedef struct pq_context pq_context;

/**
 * Builtin stuff: just register'em all.
 */
int pq_register_builtin(pq_context *ctx);

/**
 * Registers the builtin pq Types in the Context.
 */
int pq_register_builtin_types(pq_context *ctx);


/**
 * Builtin pq Values, important stuff to store directly in variables.
 */
typedef struct pq_builtin_values {
	pq_value *_nil;
	pq_value *_true;
	pq_value *_false;
	pq_value *_nan;
} pq_builtin_values;
/**
 * Registers the builtin pq Values in the Context.
 */
int pq_register_builtin_values(pq_context *ctx);


/**
 * Registers the builtin pq Functions in the Context.
 */
int pq_register_builtin_functions(pq_context *ctx);

#endif

