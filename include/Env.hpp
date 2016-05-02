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

#include <vector>

/**
 * PQ environment, with all definitions, scopes...
 */
class Env {
public:
	/**
	 * Push an integer to the call vector
	 *
	 * @param value Int value
	 */
	void pushInt (int value);

	/**
	 * Get arguments[index] as an int
	 */
	int getInt (int index);

private:
	/**
	 * GETTER for a argument pointer, already `dynamic_cast` to right type
	 *
	 * @note This is useful to unify the arguments vector access and error
	 * handling
	 */
	template<typename T>
	T *getArg (int index);

	/**
	 * Arguments used for function calling, and for result passing
	 *
	 * This 
	 */
	vector<AtomPtr> arguments;

	/**
	 * Our scope stack
	 *
	 * @note This is a vector for easy front (global scope) and back (local
	 * scope) access, and direct scope access (which is nice for debugging)
	 *
	 * Default initialization of global scope
	 */
	vector<Scope> scopeStack {1};
};
