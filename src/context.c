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

#include <pq/assert.h>
#include <pq/context.h>

#include <stdlib.h>

int pq_context_initialize(pq_context *ctx) {
	if((ctx->jit = jit_context_create()) != NULL
			&& pq_memory_manager_initialize(&ctx->memory_manager)
			&& pq_parser_initialize(&ctx->parser)
			&& pq_scope_queue_initialize(&ctx->scopes, 0)
			&& pq_scope_initialize(&ctx->env)
			&& pq_register_builtin(ctx)) {
		return 1;
	}
	return 0;
}

void pq_context_destroy(pq_context *ctx) {
	jit_context_destroy(ctx->jit);
	pq_parser_destroy(&ctx->parser);
	pq_scope_queue_destroy(ctx, &ctx->scopes);
	pq_scope_destroy(ctx, &ctx->env);
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

		case PQ_LIST: {
			pq_list list = pq_value_get_data_as(val, pq_list);
			pq_value *func = pq_eval(ctx, list.values[0]);
			pq_assert_not_error(func);
			int argc = list.size - 1;
			pq_value *ret = pq_call(ctx, func, argc, list.values + 1);
			return val;
		}

		default:
			return val;
	}
}

