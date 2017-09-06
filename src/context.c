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

#include <pq/context.h>

#include <stdlib.h>

int pq_context_initialize(pq_context *ctx) {
	// require LLVM to shutdown to avoid memory leaks
	atexit(LLVMShutdown);

	if((ctx->llvm = LLVMContextCreate()) != NULL &&
	    pq_memory_manager_initialize(&ctx->memory_manager) &&
	    pq_parser_initialize(&ctx->parser) &&
	    pq_scope_queue_initialize(&ctx->scopes, 0) &&
		pq_register_builtin_types(ctx)) {
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

pq_value *pq_context_get(const pq_context *ctx, const char *key) {
	return pq_scope_queue_get(&ctx->scopes, key);
}

void pq_context_set(pq_context *ctx, const char *key, pq_value *val) {
	pq_scope_queue_set(&ctx->scopes, key, val);
}

