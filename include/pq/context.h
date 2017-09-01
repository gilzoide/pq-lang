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

#ifndef __PQ_CONTEXT_H__
#define __PQ_CONTEXT_H__

#include <llvm-c/Core.h>

#include "parser.h"

/**
 * Pq interpreter Context, the topmost know-it-all structure.
 */
typedef struct {
	LLVMContextRef llvm;
	pq_parser parser;
} pq_context;

/**
 * Initialize a pq Context.
 *
 * @param ctx Context to be initialized.
 */
int pq_context_initialize(pq_context *ctx);

/**
 * Destroy a Context, freeing the memory used.
 *
 * @param ctx Context to be destroyed.
 */
void pq_context_destroy(pq_context *ctx);

#endif

