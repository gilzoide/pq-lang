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

#include <pq/symbol.h>

#include <string.h>

#define PQ_STRING_TABLE_INICIAL_CAPACITY 64

int pq_symbol_manager_initialize(pq_symbol_manager *symbol_manager) {
	symbol_manager->table = NULL;
	symbol_manager->symbol_count = 0;
	symbol_manager->strings_table_capacity = PQ_STRING_TABLE_INICIAL_CAPACITY;
	return (symbol_manager->strings_table = malloc(PQ_STRING_TABLE_INICIAL_CAPACITY * sizeof(char *))) != NULL;
}

void pq_symbol_manager_destroy(pq_symbol_manager *symbol_manager) {
	Word_t ret;
	JHSFA(ret, symbol_manager->table);
	int i;
	for(i = 0; i < symbol_manager->symbol_count; i++) {
		free(symbol_manager->strings_table[i]);
	}
	free(symbol_manager->strings_table);
}

/// Doubles the capacity of the Strings table when needed.
static int pq_double_table_capacity(pq_symbol_manager *symbol_manager) {
	unsigned int new_capacity = 2 * symbol_manager->strings_table_capacity;
	char **new_strings_table;
	if(new_strings_table = realloc(symbol_manager->strings_table, new_capacity * sizeof(char *))) {
		symbol_manager->strings_table = new_strings_table;
		symbol_manager->strings_table_capacity = new_capacity;
	}
	return new_strings_table != NULL;
}

pq_symbol pq_symbol_manager_symbol_from_string(pq_symbol_manager *symbol_manager, const char *str) {
	return pq_symbol_manager_symbol_from_lstring(symbol_manager, str, strlen(str));
}

pq_symbol pq_symbol_manager_symbol_from_lstring(pq_symbol_manager *symbol_manager, const char *str, size_t n) {
	Word_t *pvalue;
	JHSI(pvalue, symbol_manager->table, (unsigned char *)str, n);
	if(pvalue == PJERR) {
		return PQ_SYMBOL_NIL;
	}
	else if(*pvalue == 0) {
		if(symbol_manager->symbol_count + 1 >= symbol_manager->strings_table_capacity) {
			if(!pq_double_table_capacity(symbol_manager)) {
				return PQ_SYMBOL_NIL;
			}
		}
		if((symbol_manager->strings_table[symbol_manager->symbol_count] = strndup(str, n)) == NULL) {
			return PQ_SYMBOL_NIL;
		}
		*pvalue = ++symbol_manager->symbol_count;
	}
	return *pvalue;
}

const char *pq_symbol_manager_string_from_symbol(pq_symbol_manager *symbol_manager, pq_symbol symbol) {
	return symbol > PQ_SYMBOL_NIL && symbol <= symbol_manager->symbol_count
			? symbol_manager->strings_table[symbol - 1]
			: NULL;
}

