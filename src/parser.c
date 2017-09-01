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

#include <ctype.h>

/* PQ Expression Grammar
 * =====================
 *
 * Expr <- Sp (SExpr / Atom)
 * SExpr <- "(" Sp (Expr Sp)* ")"
 * Atom <- [^()]+
 *
 * Comment <- ";" [^\n]*
 * Sp <- (\s / Comment)*
 */
#include <pega-texto/macro-on.h>
int pq_parser_initialize(pq_parser *parser) {
	pt_rule rules[] = {
		{ "Expr", SEQ(V("Sp"), OR(V("SExpr"), V("Atom"))) },
		{ "SExpr", SEQ(L("("), V("Sp"), Q(SEQ(V("Expr"), V("Sp")), 0), L(")")) },
		{ "Atom", Q(BUT(S("()")), 1) },
		{ "Comment", SEQ(L(";"), Q(BUT(L("\n")), 0)) },
		{ "Sp", Q(OR(F(isspace), V("Comment")), 0) },
		{ NULL, NULL },
	};
	parser->grammar = pt_create_grammar(rules, 0);
	pt_validate_grammar(parser->grammar, PT_VALIDATE_ABORT);
	return parser->grammar != NULL;
}
#include <pega-texto/macro-off.h>

void pq_parser_destroy(pq_parser *parser) {
	pt_destroy_grammar(parser->grammar);
}

