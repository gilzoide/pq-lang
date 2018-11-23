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

#ifndef __PQ_SYMBOL_H__
#define __PQ_SYMBOL_H__

#include <Judy.h>

/// A symbol is just an int representing an interned string
typedef Word_t pq_symbol;

#define PQ_SYMBOL_NIL (0)

/**
 * Manages the creation and interning of Symbols.
 */
typedef struct pq_symbol_manager {
	Pvoid_t table;  ///< The internal table that relates String -> Symbol.
	unsigned long symbol_count;  ///< The number of Symbols managed.
	char **strings_table;  ///< The internal table that relates Symbol -> String.
	unsigned long strings_table_capacity;  ///< The capacity of the strings table.
} pq_symbol_manager;

/**
 * Initialize a Symbol Manager.
 */
int pq_symbol_manager_initialize(pq_symbol_manager *symbol_manager);
/**
 * Destroy the memory used by a Symbol Manager.
 */
void pq_symbol_manager_destroy(pq_symbol_manager *symbol_manager);

/**
 * Get the interned Symbol correspondent to the string.
 */
pq_symbol pq_symbol_manager_symbol_from_string(pq_symbol_manager *symbol_manager, const char *str);
/**
 * Get the interned Symbol correspondent to the string with length `n`.
 */
pq_symbol pq_symbol_manager_symbol_from_lstring(pq_symbol_manager *symbol_manager, const char *str, size_t n);
/**
 * Get the string correspondent to the interned Symbol.
 */
const char *pq_symbol_manager_string_from_symbol(pq_symbol_manager *symbol_manager, pq_symbol symbol);

#endif

