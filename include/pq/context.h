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

/** @file context.h
 * Pq interpreter Context, the topmost know-it-all structure.
 */

#ifndef __PQ_CONTEXT_H__
#define __PQ_CONTEXT_H__

#include <llvm-c/Core.h>

#include "builtin.h"
#include "cons.h"
#include "memory_manager.h"
#include "parser.h"
#include "scope.h"
#include "scope_queue.h"
#include "type.h"

/**
 * Pq interpreter Context, the topmost know-it-all structure.
 */
typedef struct pq_context {
	LLVMContextRef llvm;
	pq_parser parser;
	pq_scope_queue scopes;
	pq_builtin_types builtin_types;
	pq_builtin_values builtin_values;
	pq_memory_manager memory_manager;
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


/**
 * Get a local Value, searching for it from local (topmost) to root Scope.
 */
pq_value *pq_context_get(const pq_context *ctx, const char *key);
/**
 * Set a Value in the local (topmost) Scope.
 */
void pq_context_set(pq_context *ctx, const char *key, pq_value *val);


/**
 * Pushes a new Scope into Context.
 */
void pq_push_scope(pq_context *ctx);

#endif

