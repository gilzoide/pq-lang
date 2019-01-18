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
	return pq_vector_initialize_as(&symbol_manager->strings_table, PQ_STRING_TABLE_INICIAL_CAPACITY, char *);
}

void pq_symbol_manager_destroy(pq_symbol_manager *symbol_manager) {
	Word_t ret;
	JHSFA(ret, symbol_manager->table);
	int i;
	for(i = 0; i < symbol_manager->strings_table.size; i++) {
		free(*pq_vector_at(&symbol_manager->strings_table, i, char *));
	}
	pq_vector_destroy(&symbol_manager->strings_table);
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
		char **new_string_entry = pq_vector_push_as(&symbol_manager->strings_table, char *);
		if(new_string_entry != NULL && (*new_string_entry = strndup(str, n)) != NULL) {
			*pvalue = symbol_manager->strings_table.size;
		}
		else {
			return PQ_SYMBOL_NIL;
		}
	}
	return *pvalue;
}

const char *pq_symbol_manager_string_from_symbol(pq_symbol_manager *symbol_manager, pq_symbol symbol) {
	return symbol > PQ_SYMBOL_NIL && symbol <= symbol_manager->strings_table.size
			? *(pq_vector_at(&symbol_manager->strings_table, symbol - 1, const char *))
			: NULL;
}

