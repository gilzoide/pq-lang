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

/** @file Atom.hpp
 * Atom, abstraction for all PQ values
 */
#pragma once

#include "Exception.hpp"
#include "debug.hpp"

#include <memory>
#include <sstream>

using namespace std;

namespace pq {

class Atom;

/// Atom pointer, used in PQ
using AtomPtr = Atom *;

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
	 *
	 * @return Atom clone
	 */
	virtual AtomPtr clone () = 0;

	/**
	 * Cast stored pointer as of pointer to type T, nullptr version
	 *
	 * @return Pointer cast to T *
	 * @return nullptr if cast is impossible
	 */
	template<typename T>
	T *as () noexcept {
		return dynamic_cast<T *> (this);
	}
	/**
	 * Cast stored pointer as of pointer to type T, exception version
	 *
	 * @return Pointer cast to T *
	 *
	 * @throw pq::Exception if T isn't a valid Atom type
	 */
	template<typename T>
	T *assert () {
		if (auto ptr = this->as<T> ()) {
			return ptr;
		}
		else {
			throw PQ_API_EXCEPTION ("Atom::assert",
					"Invalid type for conversion from Atom");
		}
	}

private:
	/**
	 * Index of which scope owns this Atom
	 *
	 * This is used for memory management, so that Atoms get released as the
	 * scope dies, as described in [Memory](design/memoria.md)
	 */
	uint16_t fatherScope {0};
};

}
