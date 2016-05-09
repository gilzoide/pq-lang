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

#include <unordered_set>
#include <stack>

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
	void disposeInt (Int *ptr);

	/**
	 * Returns a string with the memory stats, including how many objects are
	 * instantiated and 
	 */
	string getMemoryStats ();
private:
	/// Int Pool
	unordered_set<Int *> IntPool;
	/// Available (unactive) Ints
	stack<Int *> availableInts;
};

}
