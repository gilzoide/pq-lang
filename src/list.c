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

#include <pq/list.h>
#include <pq/utils.h>

static inline int _pq_absolute_index(pq_list lst, int index) {
	return ((index < 0) * lst.size) + index;
}
static inline int _pq_list_index_is_inbounds(pq_list lst, int index) {
	return index >= 0 && index < lst.size;
}

pq_value *pq_list_value_at(pq_list lst, int index) {
	index = _pq_absolute_index(lst, index);
	return _pq_list_index_is_inbounds(lst, index) ? lst.values[index] : NULL;
}

pq_list pq_list_slice(pq_list lst, int from, int to) {
	from = _pq_absolute_index(lst, from);
	to = _pq_absolute_index(lst, to);

	return (pq_list){
		.values = lst.values + max_int(0, from),
		.size = CLAMP_INT(to - from, 0, lst.size - from),
		.owns_data = 0,
	};
}

