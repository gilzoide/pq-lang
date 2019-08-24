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

#ifndef __PQ_PARSER_H__
#define __PQ_PARSER_H__

#include <pega-texto.h>

#include "value.h"

/**
 * Pq Parser: a customizable parser for single expressions.
 */
typedef struct pq_parser {
	pt_grammar *grammar;

	const char *last_error;
	int last_error_line;
	int last_error_line_position;
	int last_error_position;
} pq_parser;

// Forward declarations
typedef struct pq_context pq_context;

/**
 * Initialize a pq Parser.
 */
int pq_parser_initialize(pq_parser *parser);
/**
 * Destroy a Parser, freeing the memory used.
 */
void pq_parser_destroy(pq_parser *parser);


/**
 * Read an expression, returning the parsed Value.
 */
pq_value *pq_read(pq_context *ctx, const char *str, int *num_read_chars);

#endif

