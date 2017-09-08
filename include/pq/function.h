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

/** @file function.h
 * Pq callable functions, defined either through pq, C or LLVM.
 */

#ifndef __PQ_FUNCTION_H__
#define __PQ_FUNCTION_H__

#include "value.h"

#include <stdint.h>

/**
 * Functions defined in pq: code defined by Cons Cells.
 */
typedef struct pq_function {
	pq_value *code;
	uint8_t argmin;
	uint8_t argmax;
} pq_function;

/**
 * Functions defined in C, receiving pq Values in the argv array.
 */
typedef struct pq_c_function {
	pq_c_function_ptr fptr;
	uint8_t argmin;
	uint8_t argmax;
} pq_c_function;

// Forward declarations
typedef struct pq_context pq_context;

/**
 * Registers a C Function into pq Context.
 */
pq_value *pq_register_c_function(pq_context *ctx, const char *name, pq_c_function_ptr func, uint8_t argmin, uint8_t argmax);

/**
 * Try to call the `func` value, which should be callable (either a function, macro or type).
 *
 * @param ctx  Pq Context.
 * @param func Function value to be called.
 * @param argc Number of arguments. If this number isn't between `func->argmin`
 *             and `func->argmax`, pq_call returns the according error.
 * @param argv Array of arguments to be passed.
 *
 * @return Value returned from the function, or an Error Value if there is any.
 */
pq_value *pq_call(pq_context *ctx, pq_value *func, int argc, pq_value *argv);

#endif

