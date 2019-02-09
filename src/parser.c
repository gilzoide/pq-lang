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

////////////////////////////////////////////////////////////////////////////////
//  Syntactic errors
////////////////////////////////////////////////////////////////////////////////
enum pq_parser_error {
    PQ_PARSER_EXPECTED_PARENTHESIS = 0,
    PQ_PARSER_EXPECTED_DOUBLE_QUOTE,

    PQ_PARSER_MAX,
};
const char *pq_parser_error_messages[] = {
    "Expected closing ')'",
    "Expected closing '\"'",

    "Unknown error",
};

static void on_error(const char *str, size_t pos, int code, void *data) {
    pq_context *ctx = data;
    pq_parser *parser = &ctx->parser;

    int i;
    for(i = parser->last_error_position; i < pos; i++) {
        if(str[i] == '\n') {
            parser->last_error_line++;
            parser->last_error_line_position = i;
        }
    }
    parser->last_error_position = pos;

    fprintf(stderr, "%s:%d:%ld: parser error: %s\n",
            "REPL",
            parser->last_error_line,
            pos - parser->last_error_line_position,
            pq_parser_error_messages[code < PQ_PARSER_MAX ? code : PQ_PARSER_MAX]);
}

static pt_data read_integer(const char *str, size_t start, size_t end, int argc, pt_data *argv, void *data) {
    pq_context *ctx = data;
    return (pt_data){ .p = pq_value_from_i32(ctx, atoll(str + start)) };
}
static pt_data read_float(const char *str, size_t start, size_t end, int argc, pt_data *argv, void *data) {
    pq_context *ctx = data;
    return (pt_data){ .p = pq_value_from_float(ctx, atof(str + start)) };
}
static pt_data read_symbol(const char *str, size_t start, size_t end, int argc, pt_data *argv, void *data) {
    pq_context *ctx = data;
    pq_symbol symbol = pq_symbol_from_lstring(ctx, str + start, end - start);
    return (pt_data){ .p = pq_value_from_symbol(ctx, symbol) };
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

pt_data read_string(const char *str, size_t start, size_t end, int argc, pt_data *argv, void *data) {
    pq_context *ctx = data;
    char *l;
    pq_value *val;
    if(l = malloc((argc) * sizeof(char))) {
        int i;
        for(i = 0; i < argc; i++) {
            l[i] = argv[i].c;
        }
        val = pq_value_from_lstring(ctx, l, argc);
        free(l);
    }
    else {
        val = pq_value_nil(ctx);
    }
    return (pt_data){ .p = val };
}
static pt_data forward_character(const char *str, size_t start, size_t end, int argc, pt_data *argv, void *data) {
    return (pt_data){ .c = str[start] };
}
static pt_data character(const char *str, size_t start, size_t end, int argc, pt_data *argv, void *data) {
    char c = str[start];
    if(c == '\\') {
        c = str[start + 1];
        if(isdigit(c)) {
            int i;
            c = c - '0';
            for(i = 0; i < argc; i++) {
                c <<= 3;
                c += argv[i].c - '0';
            }
        }
        else {
            switch(c) {
                case 'a': c = '\a'; break;
                case 'b': c = '\b'; break;
                case 'f': c = '\f'; break;
                case 'n': c = '\n'; break;
                case 'r': c = '\r'; break;
                case 't': c = '\t'; break;
                case 'v': c = '\v'; break;
                default: break;
            }
        }
    }
    return (pt_data){ .c = c };
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
        { "SExpr", SEQ_(&read_list, L("("), V("Sp"), Q(SEQ(V("Expr"), V("Sp")), 0), OR(L(")"), E(PQ_PARSER_EXPECTED_PARENTHESIS, NULL))) },
        { "Atom", OR(V("Float"), V("Int"), V("String"), V("Symbol")) },
        { "Symbol", Q_(&read_symbol, BUT(OR(S("();"), C(isspace))), 1) },
        // numbers
        { "Digits", Q(C(isdigit), 1) },
        { "Sign", Q(S("+-"), 0) },
        { "Int", SEQ_(&read_integer, V("Sign"), V("Digits")) },
        { "Float", SEQ_(&read_float, V("Sign"), OR(SEQ(V("FloatDot"), Q(V("FloatExp"), -1)), SEQ(V("Digits"), V("FloatExp")))) },
        { "FloatDot", SEQ(Q(C(isdigit), 0), L("."), V("Digits")) },
        { "FloatExp", SEQ(S("eE"), V("Sign"), V("Digits")) },
        // string
        { "String", OR_(&read_string,
                        SEQ(L("\""), Q(SEQ(NOT(L("\"")), V("Character")), 0), OR(L("\""), E(PQ_PARSER_EXPECTED_DOUBLE_QUOTE, NULL)))) },
        { "Character", OR_(&character,
                           SEQ(L("\\"), S("abfnrtv\'\"[]\\")),
                           SEQ(L("\\"), R("02"), R_(&forward_character, "07"), R_(&forward_character, "07")),
                           SEQ(L("\\"), R("07"), Q(R_(&forward_character, "07"), -1)),
                           ANY) },
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
    pq_parser *parser = &ctx->parser;
    parser->last_error = NULL;
    parser->last_error_line = 0;
    parser->last_error_position = 0;
    parser->last_error_line_position = 0;

    pt_match_options opts = { .userdata = ctx, .on_error = &on_error };
    pt_match_result res = pt_match_grammar(parser->grammar, str, &opts);

    switch(res.matched) {
        case PT_MATCHED_ERROR:
            return pq_value_error(ctx, "Parser error!");

        default:
            return res.data.p ? res.data.p : pq_value_nil(ctx);
    }
}

void pq_parser_destroy(pq_parser *parser) {
    pt_destroy_grammar(parser->grammar);
}

