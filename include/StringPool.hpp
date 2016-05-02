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

#include <string>
#include <unordered_set>

using namespace std;

namespace pq {

/**
 * A string pool!
 *
 * String interning tool, for diferent uses across PQ (symbols, function
 * descriptions, maybe strings even!)
 *
 * @note As strings have destructors, and their canonical representations are
 * const ptrs, and sets destroys it's contents, don't worry, it's all freed =]
 */
class StringPool {
public:
	/**
	 * Get the canonical representation of string s
	 *
	 * @warning The canonical representation is only valid for a single object,
	 * don't mix different StringPools for same usage
	 */
	const char *get (const string& s);

private:
	/// The pool itself
	unordered_set<string> pool;
};

}
