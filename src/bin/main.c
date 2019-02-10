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

#include <replxx.h>

#include <stdio.h>
#include <stdlib.h>

#define HISTORY_FILE ".pq_history"
#define PROMPT_COLOR "\x1b[32m"
#define NORMAL_COLOR "\x1b[0m"
#define ERROR_COLOR "\x1b[1;31m"

void repl(pq_context *ctx) {
	Replxx *replxx = replxx_init();
	if(replxx == NULL) {
		fprintf(stderr, "Error initializing replxx\n");
		return;
	}
	replxx_history_load(replxx, HISTORY_FILE);

	pq_value *val;
	const char *line;
	do {
		line = replxx_input(replxx, PROMPT_COLOR "pq" NORMAL_COLOR "> ");
		if(line == NULL) break;
		replxx_history_add(replxx, line);
		if((val = pq_read(ctx, line, NULL)) && (val = pq_eval(ctx, val))) {
			pq_print(ctx, val);
			fputc('\n', stdout);
		}
	} while(val);

	replxx_history_save(replxx, HISTORY_FILE);
	replxx_end(replxx);
}

char *readfile(const char *filename) {
	FILE *fp = fopen(filename, "r");
	assert(fp != NULL && "Error reading file");
	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char *buffer = malloc((size + 1) * sizeof(char));
	assert(buffer && "[readfile] Couldn't malloc buffer");
	fread(buffer, sizeof(char), size, fp);
	buffer[size] = '\0';
	fclose(fp);
	return buffer;
}

void run_script(pq_context *ctx, const char *filename) {
    char *contents = readfile(filename);
    const char *current = contents;
    int num_read_chars;
	pq_value *val;
    do {
        if((val = pq_read(ctx, current, &num_read_chars)) && (val = pq_eval(ctx, val))) {
            if(pq_is_error(val)) {
                pq_print(ctx, val);
                fputc('\n', stdout);
                break;
            }
            current += num_read_chars;
        }
        else {
            break;
        }
    } while(num_read_chars);
    free(contents);
}

int main(int argc, char **argv) {
	pq_context ctx;
	if(!pq_context_initialize(&ctx)) {
		fprintf(stderr, "Error initializing pq context\n");
		return -1;
	}

    if(argc > 1) {
        run_script(&ctx, argv[1]);
    }
    else {
        repl(&ctx);
    }

	pq_context_destroy(&ctx);
	return 0;
}
