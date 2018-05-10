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
	return (ctx->jit = jit_context_create()) != NULL
			&& pq_symbol_manager_initialize(&ctx->symbol_manager)
			&& pq_memory_manager_initialize(&ctx->memory_manager)
			&& pq_parser_initialize(&ctx->parser)
			&& pq_scope_queue_initialize(&ctx->scopes, 0)
			&& pq_scope_initialize(&ctx->env)
			&& pq_register_builtin(ctx);
}

void pq_context_destroy(pq_context *ctx) {
	jit_context_destroy(ctx->jit);
	pq_parser_destroy(&ctx->parser);
	pq_scope_queue_destroy(ctx, &ctx->scopes);
	pq_scope_destroy(ctx, &ctx->env);
	pq_memory_manager_destroy(ctx, &ctx->memory_manager);
	pq_symbol_manager_destroy(&ctx->symbol_manager);
}

pq_value *pq_context_get(pq_context *ctx, const char *key) {
	pq_symbol symbol = pq_symbol_from_string(ctx, key);
	return pq_context_get_symbol(ctx, symbol);
}

pq_value *pq_context_get_symbol(pq_context *ctx, pq_symbol symbol) {
	pq_value *val = pq_scope_queue_get(&ctx->scopes, symbol);
	return val ? val : pq_value_ferror(ctx, "couldn't find value for symbol \"%s\"",
			pq_string_from_symbol(ctx, symbol));
}

void pq_context_set(pq_context *ctx, const char *key, pq_value *val) {
	pq_symbol symbol = pq_symbol_from_string(ctx, key);
	return pq_context_set_symbol(ctx, symbol, val);
}

void pq_context_set_symbol(pq_context *ctx, pq_symbol symbol, pq_value *val) {
	pq_scope_queue_set(&ctx->scopes, symbol, val);
}

pq_symbol pq_symbol_from_string(pq_context *ctx, const char *str) {
	return pq_symbol_manager_symbol_from_string(&ctx->symbol_manager, str);
}

pq_symbol pq_symbol_from_lstring(pq_context *ctx, const char *str, size_t n) {
	return pq_symbol_manager_symbol_from_lstring(&ctx->symbol_manager, str, n);
}

const char *pq_string_from_symbol(pq_context *ctx, pq_symbol symbol) {
	return pq_symbol_manager_string_from_symbol(&ctx->symbol_manager, symbol);
}

void pq_push_scope(pq_context *ctx) {
	pq_scope_queue_push(&ctx->scopes);
}

pq_value *pq_eval(pq_context *ctx, pq_value *val) {
	if(val == NULL) return pq_value_nil(ctx);
	switch(val->type->kind) {
		case PQ_SYMBOL:
			return pq_context_get_symbol(ctx, pq_value_get_data_as(val, pq_symbol));

		case PQ_LIST: {
			pq_list list = pq_value_get_data_as(val, pq_list);
			pq_value *func = pq_eval(ctx, list.values[0]);
			pq_assert_not_error(func);
			int argc = list.size - 1;
			pq_value *ret = pq_call(ctx, func, argc, list.values + 1);
			return ret;
		}

		default:
			return val;
	}
}

