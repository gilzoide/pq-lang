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

#include "Symbol.hpp"

#include <unordered_map>

using namespace std;

namespace pq {

/**
 * 'symbol -> Func' map/table
 */
using symTable = unordered_map<symbol, AtomPtr>;

/**
 * 'symbol -> Func' hash table
 */
class Scope {
public:
	/**
	 * Insert an Atom in key 'sym'
	 *
	 * @param sym Key symbol
	 * @param atom Value
	 *
	 * @return If insertion was possible or not
	 */
	bool insert (const string& sym, AtomPtr atom);

	/**
	 * GETTER for symbol
	 *
	 * @param Symbol for lookup
	 *
	 * @return Atom corresponding to symbol, if it exists
	 * @return Null pointer (nullptr) otherwise
	 */
	AtomPtr operator[] (const string& sym);

protected:
	/// The inner symbol/value table
	symTable table;
};

}
