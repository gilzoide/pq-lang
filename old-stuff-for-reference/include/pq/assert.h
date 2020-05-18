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

/** @file assert.h
 * Value assertions to be used inside C Functions/Macros, which returns a
 * `pq_value`.
 * All macros return the Error produced on assertion error.
 */

#ifndef __PQ_ASSERT_H__
#define __PQ_ASSERT_H__

/**
 * Assert that a Value is not an Error.
 */
#define pq_assert_not_error(val) \
	if(pq_is_error(val)) return val

/**
 * Assert that a Value is of some type, using the `pq_is_<type>` functions.
 */
#define pq_assert_type(ctx, val, _type) \
	if(!pq_is_ ## _type (val)) \
		return pq_value_ferror(ctx, "Type error: expected " #_type ", found %s", val->type->name)

/**
 * Assert that the argument `i` of `argv` is of some type, using the
 * `pq_is_<type>` functions.
 */
#define pq_assert_arg_type(ctx, argv, i, _type) \
	if(!pq_is_ ## _type (argv[i])) \
		return pq_value_ferror(ctx, "Invalid argument %d type: expected " #_type ", found %s", \
				i, argv[i]->type->name)

#endif

