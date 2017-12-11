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

#include <pq/parser.h>
#include <pq/context.h>

#include <ctype.h>
#include <stdlib.h>

static pt_data read_integer(const char *str, size_t start, size_t end, int argc, pt_data *argv, void *data) {
	pq_context *ctx = data;
	return (pt_data){ .p = pq_value_from_int(ctx, atoll(str + start), 32) };
}
static pt_data read_symbol(const char *str, size_t start, size_t end, int argc, pt_data *argv, void *data) {
	pq_context *ctx = data;
	return (pt_data){ .p = pq_value_from_lstring(ctx, str + start, end - start) };
}
static pt_data read_list(const char *str, size_t start, size_t end, int argc, pt_data *argv, void *data) {
	pq_context *ctx = data;
	int i;
	pq_value *list = ctx->builtin_values._nil;
	for(i = argc - 1; i >= 0; i--) {
		list = pq_value_cons(ctx, (pq_value *) argv[i].p, list);
	}
	return (pt_data){ .p = list };
}

/* PQ Expression Grammar
 * =====================
 *
 * Axiom <- Sp (!. / Expr)
 * Expr <- SExpr / Atom
 * SExpr <- "(" Sp (Expr Sp)* ")"
 * Atom <- Int / Symbol
 * Symbol <- [^()\s]+
 *
 * Int <- [+-]? \d+
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
		{ "Atom", OR(V("Int"), V("Symbol")) },
		{ "Symbol", Q_(&read_symbol, BUT(OR(S("();"), F(isspace))), 1) },
		{ "Int", SEQ_(&read_integer, Q(S("+-"), 0), Q(F(isdigit), 1)) },
		{ "Comment", SEQ(L(";"), Q(BUT(L("\n")), 0)) },
		{ "Sp", Q(OR(F(isspace), V("Comment")), 0) },
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
	// TODO: error handling
	if(res.matched > 0) {
		return res.data.p ? res.data.p : pq_value_nil(ctx);
	}
	else {
		return pq_value_error(ctx, "Erro no parse!");
	}
}

void pq_parser_destroy(pq_parser *parser) {
	pt_destroy_grammar(parser->grammar);
}

