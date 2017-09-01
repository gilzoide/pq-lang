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

	if((ctx->llvm = LLVMContextCreate()) != NULL && pq_parser_initialize(&ctx->parser)) {
		return 0;
	}
	return 1;
}

void pq_context_destroy(pq_context *ctx) {
	LLVMContextDispose(ctx->llvm);
	pq_parser_destroy(&ctx->parser);
}
