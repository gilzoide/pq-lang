/* PQ programming language backend
 * Copyright (C) <2016> <gilzoide>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Any bugs should be reported to <gilzoide@gmail.com>
 */

/** @file Cons.hpp
 * Pair of Atoms
 */
#pragma once

#include "Atom.hpp"

namespace pq {

/**
 * Cons cells, an atom and another Cons cell as continuation
 *
 * These are the cells used to make singly linked lists. If you want/need easy
 * access to the last element (as well as methods for accessing any element),
 * use List instead.
 */
class Cons : public Atom {
public:
	/**
	 * Ctor, with both parameters required
	 *
	 * Use nullptr in second to make a single element Cons list
	 *
	 * @param first First element
	 * @param second Second element
	 */
	Cons (AtomPtr first, AtomPtr second);

	/**
	 * Ctor
	 */
	~Cons ();

	/**
	 * Clone method override
	 *
	 * @return Cons clone
	 */
	AtomPtr clone () override;

	/**
	 * Make a new Cons cell with value and append it as second, and return
	 * it's reference
	 *
	 * Imagine you have a cons list with (1 2 3). If you call `append` on it
	 * with 4, it will create a Cons (4 nil), append it to the first Cons cell
	 * (1 (1 2 3)) and set it's second as the old value, so the new Cons list
	 * will be (1 4 2 3).
	 *
	 * @note This doesn't add the newSecond param at the end of a Cons list
	 *
	 * @param value New value to be appended
	 *
	 * @return New Cons cell reference, so it's easy to create a Cons list
	 * without iterating over it repeatedly
	 */
	Cons *append (AtomPtr value);

	/// First element (also called CAR)
	AtomPtr first;
	/// Second element (also called CDR)
	AtomPtr second;
};

}
