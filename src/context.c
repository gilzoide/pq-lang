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

#include <pq/assert.h>
#include <pq/context.h>

#include <stdlib.h>

int pq_context_initialize(pq_context *ctx) {
	// require LLVM to shutdown to avoid memory leaks
	atexit(LLVMShutdown);

	if((ctx->llvm = LLVMContextCreate()) != NULL
			&& pq_memory_manager_initialize(&ctx->memory_manager)
			&& pq_parser_initialize(&ctx->parser)
			&& pq_scope_queue_initialize(&ctx->scopes, 0)
			&& pq_register_builtin(ctx)) {
		return 1;
	}
	return 0;
}

void pq_context_destroy(pq_context *ctx) {
	LLVMContextDispose(ctx->llvm);
	pq_parser_destroy(&ctx->parser);
	pq_scope_queue_destroy(ctx, &ctx->scopes);
	pq_memory_manager_destroy(ctx, &ctx->memory_manager);
}

pq_value *pq_context_get(pq_context *ctx, const char *key) {
	pq_value *val = pq_scope_queue_get(&ctx->scopes, key);
	return val ? val : pq_value_ferror(ctx, "couldn't find value \"%s\"", key);
}

void pq_context_set(pq_context *ctx, const char *key, pq_value *val) {
	pq_scope_queue_set(&ctx->scopes, key, val);
}

void pq_push_scope(pq_context *ctx) {
	pq_scope_queue_push(&ctx->scopes);
}

pq_value *pq_eval(pq_context *ctx, pq_value *val) {
	if(val == NULL) return pq_value_nil(ctx);
	switch(val->type->kind) {
		case PQ_SYMBOL:
			return pq_context_get(ctx, pq_value_get_data_as(val, const char *));

		case PQ_CONS_CELL: {
			pq_cons_cell *cons = pq_value_get_data(val);
			pq_value *func = pq_eval(ctx, cons->first);
			pq_assert_not_error(func);
			// transform the arguments list into an array
			int argc = 0;
			pq_value *arg, **argv = NULL;
			while(!pq_is_nil(arg = cons->second)) {
				cons = pq_value_get_data(arg);
				argc++;
				argv = realloc(argv, argc * sizeof(pq_value *));
				argv[argc - 1] = cons->first;
			}
			pq_value *ret = pq_call(ctx, func, argc, argv);
			free(argv);
			return ret;
		}

		default:
			return val;
	}
}

