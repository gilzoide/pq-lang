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

#include <pq.h>

#include <stdio.h>

char buffer[1024];

int main() {
	printf("Size of Value: %ld\n", sizeof(pq_value));
	pq_context ctx;
	if(!pq_context_initialize(&ctx)) {
		fprintf(stderr, "Error initializing pq context\n");
		return -1;
	}

	pq_value *args[] = {
		pq_value_from_int(&ctx, 3, 32),
		pq_value_nil(&ctx),
		pq_value_from_int(&ctx, 2333, 32),
	};
	pq_value *res = pq_call(&ctx, pq_context_get(&ctx, "print-version"), 3, args);
	if(pq_is_error(res)) {
		printf("Error: %s\n", pq_value_get_data_as(res, char *));
	}
	printf("Result type: \"%s\"\n", res->type->name);

	res = pq_vcall(&ctx, pq_context_get(&ctx, "let"), 2, pq_value_from_string(&ctx, "oi"), pq_context_get(&ctx, "true"));
	pq_print(&ctx, res);
	puts("");
	pq_vcall(&ctx, pq_context_get(&ctx, "print"), 1, pq_read(&ctx, "(1 2 (5   .lol 6) ())"));

	pq_context_destroy(&ctx);
	return 0;
}
