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

/** @file AtomPool.hpp
 * Atom pool, an Object Pool for PQ Atoms
 */
#pragma once

#include "Atom.hpp"
#include "Int.hpp"
#include "Symbol.hpp"
#include "Table.hpp"
#include "Cons.hpp"
#include "List.hpp"
#include "ForwardStack.hpp"

#include <unordered_set>

namespace pq {

/**
 * A Object pool for PQ Atoms
 *
 * Every Env has an AtomPool like this one. That said, whenever you pass an Atom
 * to an Env, it gets to manage the Atom's memory, so you __must not__ free it
 * yourself ^^
 */
class AtomPool {
public:
	/**
	 * Dtor, actually deletes every object hold
	 */
	~AtomPool ();


	/**
	 * Delete an Atom, taking care of if it is a pooled Atom type
	 */
	void destroy (Atom *ptr);
	/**
	 * Dispose of any Atom pointer type
	 *
	 * This method claims memory of any Atom pointer type. Some of them should
	 * be placed in the pools (Int, Symbol, Strings, Cons, List...), so
	 * __all__ of them are tested before deleting the pointer.
	 *
	 * @note If you are sure of which type your pointer is, please use the
	 * corresponding overload as it's faster. It is safe to call
	 * `AtomPool.dispose (atomPtr->as<some_Atom_type> ())` even if the result is
	 * `nullptr`, as there is an overload for that ;]
	 *
	 * @warning Don't ever use 'ptr' after disposing of it, even if you know
	 * it's type and know it will be pooled
	 *
	 * @param ptr Atom pointer to be disposed
	 */
	void dispose (Atom *ptr);

	/**
	 * Returns a usable Int
	 *
	 * If there are disposed Ints, this function will pop the first one,
	 * activate and return it. Otherwise, it creates a new one and returns it.
	 *
	 * @return Usable Int
	 */
	Int *requestInt ();
	/**
	 * Dispose of an Int, returning it to the pool
	 *
	 * @param ptr Int pointer to be disposed
	 */
	void dispose (Int *ptr);
	/**
	 * Delete an Int, taking it out of the pool and actually deleting it's memory
	 *
	 * @note If ptr is an inactive Int (`ptr->isActive () == false`), it will
	 * have to be searched for and erased from availableInts. TL;DR: Don't
	 * `AtomPool.destroy (inactiveIntPtr);` =P
	 *
	 * @param ptr Int pointer that will be OBLITERATED
	 */
	void destroy (Int *ptr);

	/**
	 * Dispose overload for `nullptr`
	 *
	 * This is defined so that one can call `AtomPool.dispose (atomPtr->as<some_Atom_type> ())`
	 * and it doesn't put the pointer in the wrong place (if it was not of the
	 * right type).
	 *
	 * It actually doesn't do anything. This isn't necessarily bad, as memory
	 * will be claimed after AtomPool's destructor (AtomPool::~AtomPool).
	 *
	 * @note If you are not sure which type your pointer is and really want
	 * memory to be claimed, use the 'Atom *' overload instead
	 *
	 * @param ptr A `nullptr`
	 */
	void dispose (nullptr_t ptr);

	/**
	 * Returns a string with the memory stats, including how many objects are
	 * instantiated and how many are inactive
	 */
	string getMemoryStats ();

private:
	/// Int Pool
	unordered_set<Int *> IntPool;
	/// Stack (don't get fooled by forward_list) of available (inactive) Ints
	ForwardStack<Int *> availableInts;
};

}
