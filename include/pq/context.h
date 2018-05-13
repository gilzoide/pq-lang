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

/** @file context.h
 * Pq interpreter Context, the topmost know-it-all structure.
 */

#ifndef __PQ_CONTEXT_H__
#define __PQ_CONTEXT_H__

#include <jit/jit.h>

#include "builtin.h"
#include "memory_manager.h"
#include "parser.h"
#include "scope.h"
#include "scope_queue.h"
#include "type_manager.h"

/**
 * Pq interpreter Context, the topmost know-it-all structure.
 */
typedef struct pq_context {
	jit_context_t jit;
	pq_parser parser;
	pq_scope_queue scopes;
	pq_scope env;
	pq_builtin_values builtin_values;
	pq_memory_manager memory_manager;
	pq_symbol_manager symbol_manager;
	pq_type_manager type_manager;
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
pq_value *pq_context_get(pq_context *ctx, const char *key);
/**
 * Get a local Value by it's symbol, searching for it from local (topmost) to
 * root Scope.
 */
pq_value *pq_context_get_symbol(pq_context *ctx, pq_symbol symbol);
/**
 * Set a Value in the local (topmost) Scope.
 */
void pq_context_set(pq_context *ctx, const char *key, pq_value *val);
/**
 * Set a Value by it's symbol in the local (topmost) Scope.
 */
void pq_context_set_symbol(pq_context *ctx, pq_symbol symbol, pq_value *val);

/**
 * Get the interned Symbol correspondent to the string.
 */
pq_symbol pq_symbol_from_string(pq_context *ctx, const char *str);
/**
 * Get the interned Symbol correspondent to the string with length `n`.
 */
pq_symbol pq_symbol_from_lstring(pq_context *ctx, const char *str, size_t n);
/**
 * Get the string correspondent to the interned Symbol.
 */
const char *pq_string_from_symbol(pq_context *ctx, pq_symbol symbol);


/**
 * Pushes a new Scope into Context.
 */
void pq_push_scope(pq_context *ctx);

/**
 * Evaluate a pq value.
 *
 * @todo: explain better, as eval is very important.
 */
pq_value *pq_eval(pq_context *ctx, pq_value *val);

#endif

