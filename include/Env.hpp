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

#pragma once

#include "Scope.hpp"
#include "AtomPool.hpp"

#include <vector>

namespace pq {

/**
 * PQ environment, with all definitions, scopes...
 */
class Env {
public:
	/**
	 * Push an integer to the Atom stack
	 *
	 * @param value Int value
	 */
	void pushInt (int value);

	/**
	 * Get arguments[index] as an int
	 *
	 * @note Negative values are also allowed, and index from the end
	 *
	 * @param index Index of Int to be get
	 *
	 * @return int
	 */
	int getInt (int index);

	/**
	 * Pop a number of arguments, returning them
	 *
	 * @param number Number of arguments to be popped
	 *
	 * @return Vector with popped args
	 */
	vector<AtomPtr> popArgs (unsigned int number);

private:
	/**
	 * GETTER for a argument pointer, already `dynamic_cast` to right (T) type
	 *
	 * @note This is useful to unify the arguments vector access and error
	 * handling
	 */
	AtomPtr getArg (int index);

	/**
	 * Stack that mantains Atoms for function calling, function return 
	 */
	vector<AtomPtr> atomStack;

	/**
	 * Our scope stack
	 *
	 * @note This is a vector for easy front (global scope) and back (local
	 * scope) access, and direct scope access (which is nice for debugging)
	 *
	 * Default initialization of global scope
	 */
	vector<Scope> scopeStack {1};

	/**
	 * AtomPool, for object and memory managing
	 */
	AtomPool pool;
};

}
