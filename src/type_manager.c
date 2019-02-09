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

#include <pq/type_manager.h>
#include <pq/context.h>
#include <pq/function.h>

const char * const pq_builtin_type_names[PQ_BUILTIN_TYPE_MAX] = {
    "type",
    "error",
    "list",
    "scope",
    "nil-t",
    "symbol",
    "bool",
    "i8",
    "i16",
    "i32",
    "i64",
    "u8",
    "u16",
    "u32",
    "u64",
    "sys-char",
    "sys-short",
    "sys-int",
    "sys-long",
    "sys-uchar",
    "sys-ushort",
    "sys-uint",
    "sys-ulong",
    "float",
    "double",
    "string",
    "pointer",
    "function",
    "c-function",
    "native-function",
    "overload",
};

int pq_type_manager_initialize(pq_type_manager *type_manager) {
    type_manager->pointer_table = NULL;
    type_manager->array_table = NULL;
    type_manager->tuple_table = NULL;
    type_manager->signature_table = NULL;
    type_manager->type_kind_table = NULL;
    return pq_vector_initialize_as(&type_manager->all_types, 32, pq_type *);
}

void pq_type_manager_destroy(pq_type_manager *type_manager) {
    Word_t rc_word;
    JLFA(rc_word, type_manager->pointer_table);
    JLFA(rc_word, type_manager->array_table);
    JLFA(rc_word, type_manager->type_kind_table);
    JHSFA(rc_word, type_manager->tuple_table);
    JHSFA(rc_word, type_manager->signature_table);

    int i;
    for(i = 0; i < type_manager->all_types.size; i++) {
        pq_type_destroy(*pq_vector_at(&type_manager->all_types, i, pq_type *));
    }
    pq_vector_destroy(&type_manager->all_types);
}

pq_type *pq_get_builtin_type(pq_context *ctx, enum pq_builtin_type builtin_type) {
    PQ_ASSERT(builtin_type >= PQ_TYPE_TYPE && builtin_type < PQ_BUILTIN_TYPE_MAX, "Invalid builtin type enum value");
    return *pq_vector_at(&ctx->type_manager.all_types, builtin_type, pq_type *);
}

pq_type *pq_get_type_kind_type(pq_context *ctx, enum pq_type_kind kind) {
    PQ_ASSERT(kind == PQ_NIL || kind & PQ_KIND_VALID, "Invalid type kind enum value");
	Word_t *pvalue;
	JLG(pvalue, ctx->type_manager.type_kind_table, (Word_t)kind);
	return pvalue != NULL && pvalue != PJERR ? (pq_type *)*pvalue : NULL;
}

pq_type *pq_register_type(pq_context *ctx, const char *name, enum pq_type_kind kind,
                          jit_type_t jit_type, pq_destructor value_destructor) {
    pq_type *new_type;
    if(new_type = pq_create_type(name, kind, jit_type, value_destructor)) {
        pq_type **new_type_ptr;
        if(new_type_ptr = pq_vector_push_as(&ctx->type_manager.all_types, pq_type *)) {
            *new_type_ptr = new_type;
        }
        else {
            pq_type_destroy(new_type);
            return NULL;
        }
    }
    return new_type;
}

pq_aggregate_type *pq_register_aggregate_type(pq_context *ctx, const char *name, enum pq_type_kind kind, jit_type_t jit_type,
                                    pq_type *main_subtype, unsigned int num_subtypes, pq_type **subtypes,
                                    int metadata_size, void *metadata) {
    pq_aggregate_type *new_type;
    if(new_type = pq_create_aggregate_type(name, kind, jit_type, main_subtype,
                                           num_subtypes, subtypes, metadata_size, metadata)) {
        pq_type **new_type_ptr;
        if(new_type_ptr = pq_vector_push_as(&ctx->type_manager.all_types, pq_type *)) {
            *new_type_ptr = (pq_type *)new_type;
        }
        else {
            pq_type_destroy((pq_type *)new_type);
            return NULL;
        }
    }
    return new_type;
}

pq_type *pq_get_tuple_type(pq_context *ctx, size_t n, pq_type **types) {
    Word_t *pvalue;
    JHSI(pvalue, ctx->type_manager.tuple_table, types, n * sizeof(pq_type *));
    if(pvalue != PJERR) {
        if(*pvalue == 0) {
            jit_type_t field_types[n];
            int i;
            for(i = 0; i < n; i++) {
                field_types[i] = types[i]->jit_type;
            }
            jit_type_t tuple_jit_type = jit_type_create_struct(field_types, n, 1);
            *pvalue = (Word_t)pq_register_aggregate_type(ctx, NULL, PQ_TUPLE, tuple_jit_type,
                                                         NULL, n, types, 0, NULL);
        }
        return (pq_type *) *pvalue;
    }
    else {
        return NULL;
    }
}

pq_type *pq_get_array_type(pq_context *ctx, pq_type *elem_type) {
    Word_t *pvalue;
    JLI(pvalue, ctx->type_manager.array_table, (Word_t)elem_type);
    if(pvalue != PJERR) {
        if(*pvalue == 0) {
            jit_type_t field_types[] = {
				jit_type_create_pointer(elem_type->jit_type, 1),
                jit_type_nint,
            };
            jit_type_t array_jit_type = jit_type_create_struct(field_types, ARRAY_SIZE(field_types), 1);
            *pvalue = (Word_t)pq_register_aggregate_type(ctx, NULL, PQ_ARRAY, array_jit_type,
                                                         elem_type, 0, NULL, 0, NULL);
        }
        return (pq_type *)*pvalue;
    }
    else {
        return NULL;
    }
}

pq_type *pq_get_pointer_type(pq_context *ctx, pq_type *pointee_type) {
    Word_t *pvalue;
    JLI(pvalue, ctx->type_manager.pointer_table, (Word_t)pointee_type);
    if(pvalue != PJERR) {
        if(*pvalue == 0) {
            jit_type_t pointer_jit_type = jit_type_create_pointer(pointee_type->jit_type, 1);
            *pvalue = (Word_t)pq_register_aggregate_type(ctx, NULL, PQ_POINTER, pointer_jit_type,
                                                         pointee_type, 0, NULL, 0, NULL);
        }
        return (pq_type *)*pvalue;
    }
    else {
        return NULL;
    }
}

pq_type *pq_get_signature_type(pq_context *ctx, pq_type *return_type,
                               size_t n, pq_type **argument_types, uint8_t is_variadic) {
    Word_t *pvalue;
    int i;
    {
        size_t total_size = (1 + n) * sizeof(pq_type *) + (is_variadic != 0);
        uint8_t index[total_size];
        ((pq_type **)index)[0] = return_type;
		pq_type *arg_type;
        for(i = 0; i < n; i++) {
			if((arg_type = argument_types[i]) == NULL) {

				return NULL;
			}
            ((pq_type **)index)[i + 1] = arg_type;
        }
        if(is_variadic) {
            index[total_size - 1] = PQ_VARIADIC;
        }
        JHSI(pvalue, ctx->type_manager.signature_table, index, total_size);
    }
    if(pvalue != PJERR) {
        if(*pvalue == 0) {
            jit_type_t argument_jit_types[n];
            for(i = 0; i < n; i++) {
                argument_jit_types[i] = argument_types[i]->jit_type;
            }
            jit_type_t signature_jit_type = jit_type_create_signature(
                    is_variadic ? jit_abi_vararg : jit_abi_cdecl,
                    return_type->jit_type, argument_jit_types, n, 1);
            *pvalue = (Word_t)pq_register_aggregate_type(ctx, NULL, PQ_SIGNATURE, signature_jit_type, 
                                                         return_type, n, argument_types, 0, NULL);
        }
        return (pq_type *) *pvalue;
    }
    else {
        return NULL;
    }
}

pq_type *pq_create_struct_type(pq_context *ctx, const char *name,
                               size_t n, pq_type **field_types, pq_symbol *field_names) {
    pq_type *tuple_type = pq_get_tuple_type(ctx, n, field_types);
    if(tuple_type) {
        pq_aggregate_type *struct_type = pq_register_aggregate_type(ctx, name, PQ_STRUCT, tuple_type->jit_type,
                                                                    tuple_type, 0, NULL, n * sizeof(pq_symbol), field_types);
        /* pq_register_c_function */
        return (pq_type *)struct_type;
    }
    else return NULL;
}

