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

/** @file Code.hpp
 * Code lists, to be used in code storage and functions defined in PQ
 */
#pragma once

#include "List.hpp"
#include "Symbol.hpp"
#include "Cons.hpp"

namespace pq {

/**
 * PQ lispy code, a List with Symbols or other Lists
 */
class Code : public List {
public:
	/**
	 * Dtor, destroys Symbols
	 */
	~Code ();
	/**
	 * GETTER for the Function symbol, which is the first in the code List
	 */
	symbol getFuncSym () const;
	/**
	 * GETTER for the arguments Cons list
	 */
	Cons *getArguments () const;
};

}
