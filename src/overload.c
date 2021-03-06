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

#include <pq/overload.h>
#include <pq/type_manager.h>
#include <pq/value.h>

pq_overload pq_empty_overload(pq_context *ctx) {
	return (pq_overload){};
}

void pq_overload_destroy(pq_context *ctx, pq_overload *overload) {
	Word_t bytes, index, *pvalue;
	Pvoid_t table;
	index = 0;
	JLF(pvalue, overload->variadic_function_table, index);
	while(pvalue != NULL) {
		table = (Pvoid_t)*pvalue;
		JLFA(bytes, table);
		JLN(pvalue, overload->variadic_function_table, index);
	}
	JLFA(bytes, overload->variadic_function_table);

	index = 0;
	JLF(pvalue, overload->function_table, index);
	while(pvalue != NULL) {
		table = (Pvoid_t)*pvalue;
		JLFA(bytes, table);
		JLN(pvalue, overload->function_table, index);
	}
	JLFA(bytes, overload->function_table);
}

static inline Word_t *_pq_overload_non_variadic_function_pointer(pq_overload *overload, pq_function_metadata *metadata, pq_type *arguments_tuple_type) {
	Word_t *pvalue;
	JLI(pvalue, overload->function_table, metadata->argnum);
	if(pvalue != PJERR) {
		pvalue = (Word_t *)JudyLIns((PPvoid_t)pvalue, (Word_t)arguments_tuple_type, PJE0);
	}
	return pvalue;
}
static inline Word_t *_pq_overload_variadic_function_pointer(pq_overload *overload, pq_function_metadata *metadata, pq_type *arguments_tuple_type) {
	Word_t *pvalue;
	JLI(pvalue, overload->variadic_function_table, metadata->argnum);
	if(pvalue != PJERR) {
		pvalue = (Word_t *)JudyLIns((PPvoid_t)pvalue, (Word_t)arguments_tuple_type, PJE0);
	}
	return pvalue;
}
pq_value *pq_overload_add_function(pq_context *ctx, pq_overload *overload, pq_value *func) {
	if(func != NULL && !pq_is_function(func)) {
		return PQ_API_ERROR(ctx, "Cannot add non-function to overload");
	}
	else {
		pq_function_metadata *metadata = pq_value_get_data(func);
		if(!pq_function_may_be_overloaded(metadata)) return PQ_API_ERROR(ctx, "Overloaded functions must evaluate arguments");
		Word_t *pvalue;
		pq_type *signature = metadata->signature;
		pq_type *argument_types_tuple = signature == NULL
                                        ? NULL
                                        : pq_get_tuple_type(ctx, pq_type_get_num_arguments(signature), pq_type_get_argument_types(signature));
		if(metadata->flags & PQ_VARIADIC) {
			pvalue = _pq_overload_variadic_function_pointer(overload, metadata, argument_types_tuple);
		}
		else {
			pvalue = _pq_overload_non_variadic_function_pointer(overload, metadata, argument_types_tuple);
		}

		if(pvalue == PJERR) {
			func = PQ_API_ERROR(ctx, "Memory error in Judy insert");
		}
		else if(*pvalue != (Word_t)NULL) {
			func = PQ_API_ERROR(ctx, "Duplicate signature argument types for overload");
		}
		else {
			*pvalue = (Word_t)func;
		}
		return func;
	}
}

/**
 * Calculate a score for how much `types` match the expected argument types.
 * The bigger the value, a better match. A negative score means no match.
 */
static inline int _pq_signature_match_score(pq_type *argument_types_tuple, size_t n, pq_type **types) {
	int i, expected_argnum = pq_type_get_num_fields(argument_types_tuple);
	pq_type **expected_types = pq_type_get_field_types(argument_types_tuple);
	PQ_ASSERT(n >= expected_argnum, "[_pq_signature_match_score] Not enough types given for matching signature");
	PQ_ASSERT(expected_types, "[_pq_signature_match_score] NULL signature types");
	int score = 0;
	for(i = 0; i < expected_argnum; i++) {
		pq_type *expected = expected_types[i], *found = types[i];
		switch(pq_type_get_match(expected, found)) {
			case PQ_TYPE_MATCH_EQUAL:
				score += 3;
				break;
			case PQ_TYPE_MATCH_KIND:
				score += 1;
				break;
			case PQ_TYPE_MATCH_NONE: 
			default:
				return -1;
		}
	}
	return score;
}
static inline pq_value *_pq_overload_for_types_non_variadic(pq_context *ctx, pq_overload *overload, size_t n, pq_type **types) {
	Word_t *pvalue;
	pq_value *best_match;
	int best_score = -1, score;
	Pvoid_t by_argnum_table = overload->function_table, by_type_table;
	JLG(pvalue, by_argnum_table, n);
	if(pvalue != NULL && pvalue != PJERR ) {
		by_type_table = (Pvoid_t)*pvalue;
		Word_t index = 0;
		JLF(pvalue, by_type_table, index);
		while(pvalue != NULL) {
			score = _pq_signature_match_score((pq_type *)index, n, types);
			if(score > best_score) {
				best_score = score;
				best_match = (pq_value *)*pvalue;
			}
			JLN(pvalue, by_type_table, index);
		}
		if(best_score >= 0) return best_match;
	}
	return NULL;
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
		JLG(pvalue, by_type_table, (Word_t)NULL);
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

int pq_function_may_be_overloaded(pq_function_metadata *func_md) {
	return (func_md->flags & PQ_EVAL_ARGS) != 0;
}

int pq_overload_number_of_functions(pq_overload *overload) {
	Word_t num_non_variadic, num_variadic;
	JLC(num_non_variadic, overload->function_table, 0, -1);
	JLC(num_variadic, overload->variadic_function_table, 0, -1);
	return num_non_variadic + num_variadic;
}

pq_overload_iterator pq_overload_new_iterator(const pq_overload *overload) {
	return (pq_overload_iterator) {
		.signature_index = -1,
		.argnum_index = 0,
		.iterating_non_variadic = 1,
	};
}

static Pvoid_t _pq_overload_next_lookup_table(const pq_overload *overload, pq_overload_iterator *it) {
	Pvoid_t table = it->iterating_non_variadic ? overload->function_table : overload->variadic_function_table;
	Word_t *pvalue;
	if(it->argnum_index == -1) {
		it->argnum_index = 0;
		JLF(pvalue, table, it->argnum_index);
	}
	else {
		JLN(pvalue, table, it->argnum_index);
	}
	it->signature_index = -1;

	if(pvalue) return (Pvoid_t)*pvalue;
	else if(it->iterating_non_variadic) {
		it->iterating_non_variadic = 0;
		it->argnum_index = -1;
		return _pq_overload_next_lookup_table(overload, it);
	}
	else return NULL;
}
static Pvoid_t _pq_overload_current_lookup_table(const pq_overload *overload, const pq_overload_iterator *it) {
	Pvoid_t table = it->iterating_non_variadic ? overload->function_table : overload->variadic_function_table;
	Word_t *pvalue;
	JLG(pvalue, table, it->argnum_index);
	return pvalue ? (Pvoid_t)*pvalue : NULL;
}
pq_value *pq_overload_next_function(const pq_overload *overload, pq_overload_iterator *it) {
	Word_t *pvalue;
	Pvoid_t table;
	table = _pq_overload_current_lookup_table(overload, it);
	do {
		if(it->signature_index == -1) {
			it->signature_index = 0;
			JLF(pvalue, table, it->signature_index);
		}
		else {
			JLN(pvalue, table, it->signature_index);
		}
		if(pvalue) {
			return (pq_value *)*pvalue;
		}
		else {
			table = _pq_overload_next_lookup_table(overload, it);
		}
	} while(table);

	return NULL;
}

