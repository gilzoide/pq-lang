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

#include <pq/builtin.h>
#include <pq/assert.h>
#include <pq/context.h>
#include <pq/value.h>

#include <math.h>
#include <stdio.h>

int pq_register_builtin(pq_context *ctx) {
    return pq_register_builtin_types(ctx)
           && pq_register_builtin_values(ctx)
           && pq_register_builtin_functions(ctx);   
}

int pq_register_builtin_values(pq_context *ctx) {
    pq_value *new_val;
    if(new_val = pq_new_value_with_size(ctx, 0)) {
        new_val->type = ctx->type_manager._nil;
        ctx->builtin_values._nil = new_val;
        pq_context_set(ctx, "nil", new_val);
    }
    else return 0;

#define register_value(builtin_var, name, val) \
    if(new_val = val) { \
        ctx->builtin_values. builtin_var = new_val; \
        pq_context_set(ctx, name, new_val); \
    } \
    else return 0

    register_value(_false, "false", pq_value_from_bool(ctx, 0));
    register_value(_true, "true", pq_value_from_bool(ctx, 1));
    register_value(_nan, "nan", pq_value_from_double(ctx, NAN));
    register_value(_inf, "inf", pq_value_from_double(ctx, INFINITY));
#undef register_value
    return 1;
}

////////////////////////////////////////////////////////////////////////////////
//  Builtin value destructors
////////////////////////////////////////////////////////////////////////////////
static void _free_data(pq_context *ctx, void *data) {
    free(*((void **)data));
}

int pq_register_builtin_types(pq_context *ctx) {
    pq_type *type;
    pq_value *type_val;
    Word_t *pvalue;
    if(type = pq_register_type(ctx, "type", PQ_TYPE, NULL, NULL)) {
        ctx->type_manager._type = type;
        if(type_val = pq_value_from_type(ctx, type)) {
            pq_context_set(ctx, pq_builtin_type_names[PQ_TYPE_TYPE], type_val);
        }
        else return 0;
    }
    else return 0;

#define register_type(builtin_var, builtin_type_index, kind, jit_type, val_dtor) \
    if((type = pq_register_type(ctx, pq_builtin_type_names[builtin_type_index], kind, jit_type, val_dtor)) && (type_val = pq_value_from_type(ctx, type))) { \
        ctx->type_manager. builtin_var = type; \
        pq_context_set(ctx, pq_builtin_type_names[builtin_type_index], type_val); \
    } \
    else return 0

    register_type(_error,  PQ_TYPE_ERROR,  PQ_ERROR,  NULL,                &_free_data);
    register_type(_list,   PQ_TYPE_LIST,   PQ_LIST,   NULL, (pq_destructor)&pq_release_list);
    register_type(_scope,  PQ_TYPE_SCOPE,  PQ_SCOPE,  NULL, (pq_destructor)&pq_scope_destroy);
    register_type(_nil,    PQ_TYPE_NIL,    PQ_NIL,    jit_type_void,        NULL);
    register_type(_symbol, PQ_TYPE_SYMBOL, PQ_SYMBOL, jit_type_void_ptr,    NULL);

    register_type(_bool, PQ_TYPE_BOOL, PQ_BOOL, jit_type_sys_bool, NULL);

    register_type(_i8,  PQ_TYPE_I8,  PQ_INT, jit_type_sbyte,  NULL);
    register_type(_i16, PQ_TYPE_I16, PQ_INT, jit_type_short,  NULL);
    register_type(_i32, PQ_TYPE_I32, PQ_INT, jit_type_int,    NULL);
    register_type(_i64, PQ_TYPE_I64, PQ_INT, jit_type_long,   NULL);
    register_type(_u8,  PQ_TYPE_U8,  PQ_INT, jit_type_ubyte,  NULL);
    register_type(_u16, PQ_TYPE_U16, PQ_INT, jit_type_ushort, NULL);
    register_type(_u32, PQ_TYPE_U32, PQ_INT, jit_type_uint,   NULL);
    register_type(_u64, PQ_TYPE_U64, PQ_INT, jit_type_ulong,  NULL);

    register_type(_sys_char,   PQ_TYPE_SYS_CHAR,           PQ_INT, jit_type_sys_schar,  NULL);
    register_type(_sys_short,  PQ_TYPE_SYS_SHORT,          PQ_INT, jit_type_sys_short,  NULL);
    register_type(_sys_int,    PQ_TYPE_SYS_INT,            PQ_INT, jit_type_sys_int,    NULL);
    register_type(_sys_long,   PQ_TYPE_SYS_LONG,           PQ_INT, jit_type_sys_long,   NULL);
    register_type(_sys_uchar,  PQ_TYPE_SYS_UNSIGNED_CHAR,  PQ_INT, jit_type_sys_uchar,  NULL);
    register_type(_sys_ushort, PQ_TYPE_SYS_UNSIGNED_SHORT, PQ_INT, jit_type_sys_ushort, NULL);
    register_type(_sys_uint,   PQ_TYPE_SYS_UNSIGNED_INT,   PQ_INT, jit_type_sys_uint,   NULL);
    register_type(_sys_ulong,  PQ_TYPE_SYS_UNSIGNED_LONG,  PQ_INT, jit_type_sys_ulong,  NULL);

    register_type(_float,  PQ_TYPE_FLOAT,  PQ_FLOAT, jit_type_sys_float,  NULL);
    register_type(_double, PQ_TYPE_DOUBLE, PQ_FLOAT, jit_type_sys_double, NULL);

    register_type(_string, PQ_TYPE_STRING, PQ_STRING, jit_type_create_pointer(jit_type_ubyte, 1), &_free_data);

    register_type(_pointer, PQ_TYPE_POINTER, PQ_POINTER, jit_type_void_ptr, NULL);
    JLI(pvalue, ctx->type_manager.pointer_table, (Word_t)NULL);
    if(pvalue != PJERR) {
        *pvalue = (Word_t)type_val;
    }
    else {
        return 0;
    }

    register_type(_function,        PQ_TYPE_FUNCTION,        PQ_FUNCTION,        NULL, NULL);
    register_type(_c_function,      PQ_TYPE_C_FUNCTION,      PQ_C_FUNCTION,      NULL, NULL);
    register_type(_native_function, PQ_TYPE_NATIVE_FUNCTION, PQ_NATIVE_FUNCTION, NULL, NULL);
    register_type(_overload,        PQ_TYPE_OVERLOAD,        PQ_OVERLOAD,        NULL, (pq_destructor)&pq_overload_destroy);
#undef register_type


#define register_type_kind(kind) \
    JLI(pvalue, ctx->type_manager.type_kind_table, kind); \
    if(pvalue != PJERR && (type = pq_register_type(ctx, #kind, PQ_TYPE_KIND | kind, NULL, NULL)) \
                       && (type_val = pq_value_from_type(ctx, type))) { \
        *pvalue = (Word_t)type; \
        pq_context_set(ctx, #kind, type_val); \
    } \
    else return 0
    register_type_kind(PQ_NIL);
    register_type_kind(PQ_BOOL);
    register_type_kind(PQ_INT);
    register_type_kind(PQ_FLOAT);
    register_type_kind(PQ_SYMBOL);
    register_type_kind(PQ_STRING);
    register_type_kind(PQ_ERROR);
    register_type_kind(PQ_SCOPE);
    register_type_kind(PQ_LIST);
    register_type_kind(PQ_TUPLE);
    register_type_kind(PQ_STRUCT);
    register_type_kind(PQ_POINTER);
    register_type_kind(PQ_ARRAY);
    register_type_kind(PQ_SIGNATURE);
    register_type_kind(PQ_FUNCTION);
    register_type_kind(PQ_C_FUNCTION);
    register_type_kind(PQ_NATIVE_FUNCTION);
    register_type_kind(PQ_OVERLOAD);
    register_type_kind(PQ_TYPE);
    register_type_kind(PQ_KIND_SCALAR);
    register_type_kind(PQ_KIND_REPRESENTED_BY_LIST);
    register_type_kind(PQ_KIND_HAVE_FIELDS);
    register_type_kind(PQ_KIND_AGGREGATE_SINGLE_SUBTYPE);
    register_type_kind(PQ_KIND_AGGREGATE_MULTIPLE_SUBTYPES);
    register_type_kind(PQ_KIND_AGGREGATE_TYPE);
    register_type_kind(PQ_KIND_CALLABLE);

#undef register_type_kind
    return 1;
}

////////////////////////////////////////////////////////////////////////////////
//  Builtin functions
////////////////////////////////////////////////////////////////////////////////

static pq_value *_if(pq_context *ctx, jit_function_t jit_function, int argc, pq_value **argv) {
    if(jit_function) {
        return pq_value_error(ctx, "compiled `if` is not yet implemented");
    }
    else {
        return pq_value_as_bool(pq_eval(ctx, argv[0])) ? pq_eval(ctx, argv[1]) : pq_eval(ctx, argv[2]);
    }
}
static pq_value *_while(pq_context *ctx, jit_function_t jit_function, int argc, pq_value **argv) {
    if(jit_function) {
        return pq_value_error(ctx, "compiled `while` is not yet implemented");
    }
    else {
        pq_value *res = pq_value_nil(ctx);
        int i;
        while(pq_value_as_bool(pq_eval(ctx, argv[0])) && res->type->kind != PQ_ERROR) {
            for(i = 1; i < argc; i++) {
                res = pq_eval(ctx, argv[i]);
            }
        }
        return res;
    }
}
/// Just return the evaluation of the evaluated argument.
static pq_value *_eval(pq_context *ctx, int argc, pq_value **argv) {
    pq_value *code = pq_eval(ctx, argv[0]);
    return pq_eval(ctx, code);
}
/// Just return argument without evaluation.
static pq_value *_quote(pq_context *ctx, int argc, pq_value **argv) {
    return argv[0];
}
/// Return NULL, so the REPL quits.
static pq_value *_quit(pq_context *ctx, int argc, pq_value **argv) {
    return NULL;
}
/// Get the Type of a Value.
static pq_value *_type_of(pq_context *ctx, int argc, pq_value **argv) {
    return pq_value_from_type(ctx, argv[0]->type);
}
static pq_value *_lambda(pq_context *ctx, int argc, pq_value **argv) {
    pq_assert_arg_type(ctx, argv, 0, list);
    pq_list args = pq_value_as_list(argv[0]);
    pq_list code = (pq_list){ .values = argv + 1, .size = argc - 1, .owns_data = 0 };
    return pq_value_from_code(ctx, args, code, PQ_EVAL_ARGS | PQ_PUSH_SCOPE);
}
static pq_value *_macro(pq_context *ctx, int argc, pq_value **argv) {
    pq_assert_arg_type(ctx, argv, 0, list);
    pq_list args = pq_value_as_list(argv[0]);
    pq_list code = (pq_list){ .values = argv + 1, .size = argc - 1, .owns_data = 0 };
    return pq_value_from_code(ctx, args, code, PQ_PUSH_SCOPE);
}
////////////////////////////////////////////////////////////////////////////////

#include "core/core.h"
#include "cstd/ctype.c"

int pq_register_builtin_functions(pq_context *ctx) {
    return pq_register_compiler_macro(ctx, "if", &_if, 3, PQ_COMPILER_MACRO)
        && pq_register_compiler_macro(ctx, "while", &_while, 2, PQ_VARIADIC | PQ_COMPILER_MACRO)
        && pq_register_c_function(ctx, "quote", &_quote, 1, 0)
        && pq_register_c_function(ctx, "eval", &_eval, 1, 0)
        && pq_register_c_function(ctx, "quit", &_quit, 0, 0)
        && pq_register_c_function(ctx, "typeof", &_type_of, 1, PQ_EVAL_ARGS)
        && pq_register_c_function(ctx, "lambda", &_lambda, 2, PQ_VARIADIC)
        && pq_register_c_function(ctx, "macro", &_macro, 2, PQ_VARIADIC)
        && pq_register_core_value(ctx)
        && pq_register_core_type(ctx)
        && pq_register_core_int(ctx)
        && pq_register_core_list(ctx)
        && pq_register_core_print(ctx);
}

