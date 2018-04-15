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

#include <pq/list.h>
#include <pq/parser.h>
#include <pq/context.h>

#include <ctype.h>
#include <stdlib.h>

static pt_data read_integer(const char *str, size_t start, size_t end, int argc, pt_data *argv, void *data) {
	pq_context *ctx = data;
	return (pt_data){ .p = pq_value_from_int(ctx, atoll(str + start), 32) };
}
static pt_data read_float(const char *str, size_t start, size_t end, int argc, pt_data *argv, void *data) {
	pq_context *ctx = data;
	return (pt_data){ .p = pq_value_from_float(ctx, atof(str + start)) };
}
static pt_data read_symbol(const char *str, size_t start, size_t end, int argc, pt_data *argv, void *data) {
	pq_context *ctx = data;
	return (pt_data){ .p = pq_value_from_lstring(ctx, str + start, end - start) };
}
static pt_data read_list(const char *str, size_t start, size_t end, int argc, pt_data *argv, void *data) {
	pq_context *ctx = data;
	int i;
	pq_list list = pq_new_list_with_size(ctx, argc);
	for(i = 0; i < argc; i++) {
		list.values[i] = argv[i].p;
	}
	return (pt_data){ .p = pq_value_from_list(ctx, list) };
}

/* PQ Expression Grammar
 * =====================
 *
 * Axiom <- Sp (!. / Expr)
 * Expr <- SExpr / Atom
 * SExpr <- "(" Sp (Expr Sp)* ")"
 * Atom <- Float / Int / Symbol
 * Symbol <- [^();\s]+
 *
 * Digits <- \d+
 * Sign <- [+-]?
 * Int <- Sign Digits
 * Float <- Sign (FloatDot FloatExp? / \d+ FloatExp)
 * FloatDot <- \d* "." Digits
 * FloatExp <- [eE] Signal Digits
 *
 * Comment <- ";" [^\n]*
 * Sp <- (\s / Comment)*
 */
#include <pega-texto/macro-on.h>
int pq_parser_initialize(pq_parser *parser) {
	pt_rule rules[] = {
		{ "Axiom", SEQ(V("Sp"), OR(NOT(ANY), V("Expr"))) },
		{ "Expr", OR(V("SExpr"), V("Atom")) },
		{ "SExpr", SEQ_(&read_list, L("("), V("Sp"), Q(SEQ(V("Expr"), V("Sp")), 0), L(")")) },
		{ "Atom", OR(V("Float"), V("Int"), V("Symbol")) },
		{ "Symbol", Q_(&read_symbol, BUT(OR(S("();"), C(isspace))), 1) },
		// numbers
		{ "Digits", Q(C(isdigit), 1) },
		{ "Sign", Q(S("+-"), 0) },
		{ "Int", SEQ_(&read_integer, V("Sign"), V("Digits")) },
		{ "Float", SEQ_(&read_float, V("Sign"), OR(SEQ(V("FloatDot"), Q(V("FloatExp"), -1)), SEQ(V("Digits"), V("FloatExp")))) },
		{ "FloatDot", SEQ(Q(C(isdigit), 0), L("."), V("Digits")) },
		{ "FloatExp", SEQ(S("eE"), V("Sign"), V("Digits")) },
		// skip
		{ "Comment", SEQ(L(";"), Q(BUT(L("\n")), 0)) },
		{ "Sp", Q(OR(C(isspace), V("Comment")), 0) },
		{ NULL, NULL },
	};
	parser->grammar = pt_create_grammar(rules, 0);
	pt_validate_grammar(parser->grammar, PT_VALIDATE_ABORT);
	return parser->grammar != NULL;
}
#include <pega-texto/macro-off.h>

pq_value *pq_read(pq_context *ctx, const char *str) {
	pt_match_options opts = { .userdata = ctx };
	pt_match_result res = pt_match_grammar(ctx->parser.grammar, str, &opts);
	if(res.matched > 0) {
		return res.data.p ? res.data.p : pq_value_nil(ctx);
	}
	else {
		// TODO: error handling
		return pq_value_error(ctx, "Parse error");
	}
}

void pq_parser_destroy(pq_parser *parser) {
	pt_destroy_grammar(parser->grammar);
}

