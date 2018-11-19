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
	Word_t bytes, index = 0, *pvalue;
	Pvoid_t table;
	JLF(pvalue, overload->variadic_function_table, index);
	while(pvalue != NULL) {
		table = (Pvoid_t)*pvalue;
		JLFA(bytes, table);
		JLN(pvalue, overload->variadic_function_table, index);
	}
	JLFA(bytes, overload->variadic_function_table);
	JLFA(bytes, overload->function_table);
}

static inline Word_t *_pq_overload_non_variadic_function_pointer(pq_overload *overload, pq_type *argument_types_tuple) {
	Word_t *pvalue;
	JLI(pvalue, overload->function_table, (Word_t)argument_types_tuple);
	return pvalue;
}
static inline Word_t *_pq_overload_variadic_function_pointer(pq_overload *overload, pq_function_metadata *metadata, pq_type *argument_types_tuple) {
	Word_t *pvalue;
	JLI(pvalue, overload->variadic_function_table, metadata->argnum);
	if(pvalue != PJERR) {
		pvalue = (Word_t *)JudyLIns((PPvoid_t)pvalue, (Word_t)argument_types_tuple, PJE0);
	}
	return pvalue;
}
pq_value *pq_overload_add_function(pq_context *ctx, pq_overload *overload, pq_value *func) {
	if(func != NULL && pq_is_function(func)) {
		Word_t *pvalue;
		pq_function_metadata *metadata = pq_value_get_data(func);
		pq_type *signature = metadata->signature;
		pq_type *argument_types_tuple = signature == NULL
		                                ? NULL
		                                : pq_get_tuple_type(ctx, pq_type_get_num_arguments(signature), pq_type_get_argument_types(signature));
		if(signature != NULL && metadata->flags & PQ_VARIADIC) {
			pvalue = _pq_overload_variadic_function_pointer(overload, metadata, argument_types_tuple);
		}
		else {
			pvalue = _pq_overload_non_variadic_function_pointer(overload, argument_types_tuple);
		}
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

static inline pq_value *_pq_overload_for_types_non_variadic(pq_context *ctx, pq_overload *overload, size_t n, pq_type **types) {
	Word_t *pvalue;
	pq_type *argument_types_tuple = pq_get_tuple_type(ctx, n, types);
	JLG(pvalue, overload->function_table, (Word_t)argument_types_tuple);
	return pvalue == NULL ? NULL : (pq_value *)*pvalue;
}
static inline pq_value *_pq_overload_for_types_variadic(pq_context *ctx, pq_overload *overload, size_t n, pq_type **types) {
	Word_t *pvalue;
	pq_type *argument_types_tuple;
	Pvoid_t by_argnum_table = overload->variadic_function_table, by_type_table;
	JLL(pvalue, by_argnum_table, n);
	while(pvalue != NULL) {
		by_type_table = (Pvoid_t)*pvalue;
		argument_types_tuple = pq_get_tuple_type(ctx, n, types);
		JLG(pvalue, by_type_table, (Word_t)argument_types_tuple);
		if(pvalue != NULL) {
			return (pq_value *)*pvalue;
		}
		JLP(pvalue, by_argnum_table, n);
	}
	return NULL;
}
static inline pq_value *_pq_overload_for_any_type(pq_overload *overload) {
	Word_t *pvalue;
	JLG(pvalue, overload->function_table, (Word_t)NULL);
	return pvalue == NULL ? NULL : (pq_value *)*pvalue;
}
pq_value *pq_overload_for_types(pq_context *ctx, pq_overload *overload, size_t n, pq_type **types) {
	pq_value *val;
	val = _pq_overload_for_types_non_variadic(ctx, overload, n, types);
	if(val == NULL) {
		val = _pq_overload_for_types_variadic(ctx, overload, n, types);
		if(val == NULL) {
			val = _pq_overload_for_any_type(overload);
			if(val == NULL) {
				val = pq_value_nil(ctx);
			}
		}
	}
	return val;
}

