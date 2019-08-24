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

#include <ctype.h>

int pq_register_cstd_ctype_function(pq_context *ctx) {
	pq_type *int_type = pq_get_builtin_type(ctx, PQ_TYPE_SYS_INT);
	pq_type *ctype_sig = pq_get_signature_type(ctx, int_type, 1, (pq_type *[]){ int_type }, 0);
	return pq_register_native_function(ctx, "isalnum", &isalnum, ctype_sig)
	    && pq_register_native_function(ctx, "isalpha", &isalpha, ctype_sig)
	    && pq_register_native_function(ctx, "iscntrl", &iscntrl, ctype_sig)
	    && pq_register_native_function(ctx, "isdigit", &isdigit, ctype_sig)
	    && pq_register_native_function(ctx, "isgraph", &isgraph, ctype_sig)
	    && pq_register_native_function(ctx, "islower", &islower, ctype_sig)
	    && pq_register_native_function(ctx, "isprint", &isprint, ctype_sig)
	    && pq_register_native_function(ctx, "ispunct", &ispunct, ctype_sig)
	    && pq_register_native_function(ctx, "isspace", &isspace, ctype_sig)
	    && pq_register_native_function(ctx, "isupper", &isupper, ctype_sig)
	    && pq_register_native_function(ctx, "isxdigit", &isxdigit, ctype_sig);
}
