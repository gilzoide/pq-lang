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

// Forward declaration
class ConsListIterator;

/**
 * Cons cells, an atom and another Cons cell as continuation
 *
 * These are the cells used to make singly linked lists. If you want/need easy
 * access to the last element (as well as methods for accessing any element),
 * use List instead.
 */
class Cons {
public:
	/**
	 * Ctor
	 */
	Cons ();
	/**
	 * Ctor, with both parameters required
	 *
	 * Sets DEFINED in AtomFlags
	 *
	 * Use nullptr in second to make a single element Cons list
	 *
	 * @param first First element
	 * @param second Cons continuation
	 */
	Cons (AtomPtr elem, Cons *next = nullptr);

	/**
	 * Dtor
	 */
	~Cons ();

	/**
	 * Clone a Cons list (entire list, O(N) ), for pass by Value semantics
	 */
	Cons *clone ();

	/**
	 * Make a new Cons cell with elen, append it as next, and return it's
	 * reference
	 *
	 * Imagine you have a cons list with (1 2 3). If you call `append` on it
	 * with 4, it will create a Cons (4 nil), append it to the first Cons cell
	 * (1 (1 2 3)) and set it's second as the old value, so the new Cons list
	 * will be (1 4 2 3).
	 *
	 * @note This doesn't add the newSecond param at the end of a Cons list,
	 * but rather inserts a cell in `this`
	 *
	 * @warning Cons' destructor doesn't `delete` its values, so you __must__
	 * take care of the memory allocated and returned by this method
	 *
	 * @param elem New element to be appended
	 *
	 * @return New Cons cell reference, so it's easy to create a Cons list
	 * without iterating over it repeatedly
	 */
	Cons *append (AtomPtr elem);
	/**
	 * Append existing Cons cell as `next`, setting `cell.next` as old `next`
	 *
	 * @note We use this so we can pool Cons cells
	 *
	 * @warning This method overrides `cell.next` field
	 *
	 * @param cell Cons cell to be appended
	 *
	 * @return cell, the Cons cell that was just appended
	 */
	Cons *append (Cons *cell);

	/**
	 * Make a new Cons cell with value, append it as next, and return it's
	 * reference
	 *
	 * @param elem New element to be prepended
	 *
	 * @return New Cons cell reference, so it's easy to create a stack
	 */
	Cons *prepend (AtomPtr elem);
	/**
	 * Prepend existing Cons cell as `this.next`, setting `cell.next` as `this`
	 *
	 * @note We use this so we can pool Cons cells
	 *
	 * @warning This method overrides `cell.next` field
	 *
	 * @param elem Cons cell to be preppended
	 *
	 * @return cell, the Cons cell that was just prepended
	 */
	Cons *prepend (Cons *cell);

	/**
	 * Reset Cons cell, making both `elem` and `next` as nullptr
	 *
	 * @warning This method don't take care of previous variables in pointers
	 * `elem` nor `next`
	 */
	void reset ();

	/**
	 * Create begin iterator, for iterating in a `for`
	 */
	ConsListIterator begin ();
	/**
	 * Create end iterator (nullptr), for iterating in a `for`
	 */
	ConsListIterator end ();

	/// Cell element (also called CAR)
	AtomPtr elem {nullptr};
	/// Next cell (also called CDR)
	Cons *next {nullptr};
};


/**
 * Cons list iterator, so we can iterate a Cons with a `for`
 */
class ConsListIterator {
public:
	/**
	 * Ctor
	 *
	 * @param initial Initial Cons cell
	 */
	ConsListIterator (Cons *initial);

	/**
	 * Verify if iterators are different
	 *
	 * @param other Other iterator
	 *
	 * @return If `this.it != other.it`
	 */
	bool operator!= (const ConsListIterator& other) const;

	/**
	 * Advance iterator, next Cons cell
	 *
	 * @return New iterator (actually same iterator, but advanced)
	 */
	const ConsListIterator& operator++ ();

	/**
	 * Derreference iterator
	 *
	 * @return Atom in Cons cell (CAR)
	 */
	AtomPtr operator* () const;

private:
	/// Inner iterator
	Cons *it;
};

}
