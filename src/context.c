/*
 * Copyright 2017-2019 Gil Barbosa Reis <gilzoide@gmail.com>
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
	       && pq_type_manager_initialize(&ctx->type_manager)
	       && pq_parser_initialize(&ctx->parser)
	       && pq_scope_stack_initialize(&ctx->scopes, 0)
	       && pq_scope_initialize(&ctx->env)
	       && pq_register_builtin(ctx);
}

void pq_context_destroy(pq_context *ctx) {
	jit_context_destroy(ctx->jit);
	pq_parser_destroy(&ctx->parser);
	pq_scope_stack_destroy(ctx, &ctx->scopes);
	pq_scope_destroy(ctx, &ctx->env);
	pq_memory_manager_destroy(ctx, &ctx->memory_manager);
	pq_symbol_manager_destroy(&ctx->symbol_manager);
	pq_type_manager_destroy(&ctx->type_manager);
}

pq_value *pq_context_get(pq_context *ctx, const char *key) {
	return pq_context_get_symbol(ctx, pq_symbol_from_string(ctx, key));
}

pq_value *pq_context_get_symbol(pq_context *ctx, pq_symbol symbol) {
	pq_value *val = pq_scope_stack_get(&ctx->scopes, symbol);
	return val ? val : pq_value_ferror(ctx, "couldn't find value for symbol \"%s\"",
			pq_string_from_symbol(ctx, symbol));
}

void pq_context_set(pq_context *ctx, const char *key, pq_value *val) {
	return pq_context_set_symbol(ctx, pq_symbol_from_string(ctx, key), val);
}

void pq_context_set_symbol(pq_context *ctx, pq_symbol symbol, pq_value *val) {
	pq_scope_stack_set(&ctx->scopes, symbol, val);
}

void pq_context_set_global_symbol(pq_context *ctx, pq_symbol sym, pq_value *val) {
	pq_scope_stack_set_root(&ctx->scopes, sym, val);
}

pq_value *pq_context_set_function(pq_context *ctx, const char *key, pq_value *val) {
	pq_symbol symbol = pq_symbol_from_string(ctx, key);
	return pq_context_set_function_symbol(ctx, symbol, val);
}

pq_value *pq_context_set_function_symbol(pq_context *ctx, pq_symbol sym, pq_value *val) {
	if(!pq_is_function(val)) {
		return pq_value_error(ctx, "Value must be a function");
	}
	pq_function_metadata *func_md = pq_value_get_data(val);
	if(func_md->symbol != PQ_SYMBOL_NIL) {
		return pq_value_ferror(ctx, "Trying to register function already registered as '%s'", pq_string_from_symbol(ctx, func_md->symbol));
	}
	func_md->symbol = sym;
	pq_value *previous_value = pq_scope_stack_get_root(&ctx->scopes, sym);
	if(previous_value == NULL) {
		pq_context_set_global_symbol(ctx, sym, val);
		return val;
	}
	else if(pq_is_overload(previous_value)) {
		return pq_overload_add_function(ctx, (pq_overload *)pq_value_get_data(previous_value), val);
	}
	else if(pq_is_function(previous_value)) { 
		if(pq_function_may_be_overloaded(pq_value_get_data(previous_value))) {
			pq_value *overload_value = pq_value_from_overload(ctx, pq_empty_overload(ctx));
			pq_overload *overload = pq_value_get_data(overload_value);
			overload->symbol = sym;
			pq_overload_add_function(ctx, overload, previous_value);
			pq_context_set_global_symbol(ctx, sym, overload_value);
			return pq_overload_add_function(ctx, overload, val);
		}
		else {
			pq_context_unset_function_symbol(ctx, sym);
		}
	}
	pq_context_set_global_symbol(ctx, sym, val);
	return val;
}

pq_value *pq_context_unset_function(pq_context *ctx, const char *key) {
	return pq_context_unset_function_symbol(ctx, pq_symbol_from_string(ctx, key));
}

pq_value *pq_context_unset_function_symbol(pq_context *ctx, pq_symbol sym) {
	return pq_value_nil(ctx); //TODO
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
	pq_scope_stack_push(&ctx->scopes);
}

