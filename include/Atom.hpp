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

#include <memory>

using namespace std;

/**
 * Atom, the abstract class for PQ values
 *
 * Everything is a function in PQ, so we use this OO to help everything work
 * smooth and easily
 */
class Atom {
public:
	/**
	 * Virtual Dtor, we must have one
	 */
	virtual ~Atom ();

	/**
	 * Clone method, needed for passing by value semantics
	 *
	 * @note Children must implement this, so our system works
	 */
	virtual Atom *clone () = 0;

	/**
	 * Method that returns how many arguments are expected by Atom
	 *
	 * When the value returned is positive (say, +N), function expects exactly
	 * N arguments. When negative (say, -N), function expects __at least__ N
	 * arguments (more are allowed)
	 *
	 * @note As everything is a function in PQ, even simple values need this
	 * function implemented. Default implementation just returns 0 though, so
	 * don't worry.
	 *
	 * @return Number of expected arguments. 0 by default
	 */
	virtual int getExpectedArgs ();

protected:
};

/// Atom pointers used by the backend, reference counted
using AtomPtr = shared_ptr<Atom>;
