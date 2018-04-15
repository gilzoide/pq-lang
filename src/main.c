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

#include <pq.h>

#include <stdio.h>
#include <stdlib.h>

#define LINE_SIZE 1024
char buffer[LINE_SIZE];

int main() {
	pq_context ctx;
	if(!pq_context_initialize(&ctx)) {
		fprintf(stderr, "Error initializing pq context\n");
		return -1;
	}

	pq_value *val;
	do {
		printf("> ");
		if(!fgets(buffer, LINE_SIZE, stdin)) break;
		if((val = pq_read(&ctx, buffer)) && (val = pq_eval(&ctx, val))) {
			pq_print(&ctx, val);
			fputc('\n', stdout);
		}
	} while(val);

	pq_context_destroy(&ctx);
	return 0;
}
