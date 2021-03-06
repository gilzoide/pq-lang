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

/** @file core.h
 * Core functions register functions.
 */

#ifndef __PQ_CORE_H__
#define __PQ_CORE_H__

// Forward declarations
typedef struct pq_context pq_context;

/// Register value builtin functions.
int pq_register_core_value(pq_context *ctx);
/// Register type builtin functions.
int pq_register_core_type(pq_context *ctx);
/// Register integer types functions.
int pq_register_core_int(pq_context *ctx);
/// Register list type functions.
int pq_register_core_list(pq_context *ctx);
/// Register tuple type functions.
int pq_register_core_tuple(pq_context *ctx);
/// Register print function.
int pq_register_core_print(pq_context *ctx);

#endif

