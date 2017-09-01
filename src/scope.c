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

#include <Judy.h>

#include <pq/scope.h>

int pq_scope_initialize(pq_scope *scope) {
	*scope = NULL;
}

void pq_scope_destroy(pq_scope *scope) {
	Word_t bytes;
	JSLFA(bytes, *scope);
}

pq_value *pq_scope_get(const pq_scope *scope, const char *key) {
	Word_t *pvalue;
	JSLG(pvalue, *scope, key);
	return (pq_value *) pvalue;
}

pq_value *pq_scope_set(pq_scope *scope, const char *key, pq_value *val) {
	Word_t *pvalue;
	JSLI(pvalue, *scope, key);
	pq_value *previous_value = (pq_value *) *pvalue;
	*pvalue = (Word_t) val;
	return previous_value;
}

