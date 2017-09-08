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
	pq_context ctx;
	if(!pq_context_initialize(&ctx)) {
		fprintf(stderr, "Error initializing pq context\n");
		return -1;
	}

	pq_value *res = pq_call(&ctx, pq_context_get(&ctx, "print-version"), 0, NULL);
	if(res->type->kind == PQ_ERROR) {
		printf("Error: %s\n", (char *) res->data);
	}
	printf("Result type: \"%s\"\n", res->type->name);

	pq_context_destroy(&ctx);
	return 0;
}
