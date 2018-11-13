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

#include <pq/overload.h>
#include <pq/type_manager.h>
#include <pq/value.h>

pq_overload pq_empty_overload(pq_context *ctx) {
	return (pq_overload){};
}

void pq_overload_destroy(pq_context *ctx, pq_overload *overload) {
	Word_t bytes;
	JLFA(bytes, overload->function_table);
}

pq_value *pq_overload_add_function(pq_context *ctx, pq_overload *overload, pq_value *func) {
	if(func != NULL && pq_is_function(func)) {
		Word_t *pvalue;
		pq_type *signature = pq_value_get_data_as(func, pq_function_metadata).signature;
		pq_type *argument_types_tuple = signature == NULL ? NULL : pq_get_tuple_type(ctx, pq_type_get_num_arguments(signature), pq_type_get_argument_types(signature));
		JLI(pvalue, overload->function_table, (Word_t)argument_types_tuple);
		if(pvalue == PJERR) {
			func = pq_value_error(ctx, "Memory error in Judy insert");
		}
		else if(*pvalue != (Word_t)NULL) {
			func = pq_value_error(ctx, "Duplicate signature argument types for overload");
		}
		else {
			*pvalue = (Word_t)func;
		}
		return func;
	}
	else {
		return pq_value_error(ctx, "Cannot add non-function to overload");
	}
}

pq_value *pq_overload_for_types(pq_context *ctx, pq_overload *overload, size_t n, pq_type **types) {
	Word_t *pvalue;
	pq_type *argument_types_tuple = pq_get_tuple_type(ctx, n, types);
	JLG(pvalue, overload->function_table, (Word_t)argument_types_tuple);
	pq_value *val;
	if(pvalue == NULL) {
		// try finding fallback
		JLG(pvalue, overload->function_table, (Word_t)NULL);
		if(pvalue == NULL) {
			val = pq_value_nil(ctx);
		}
		else {
			val = (pq_value *)*pvalue;
		}
	}
	else {
		val = (pq_value *)*pvalue;
	}
	return val;
}

