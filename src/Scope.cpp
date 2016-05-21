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

#include "Scope.hpp"

namespace pq {

bool Scope::insert (const string& sym, AtomPtr atom) {
	// get canonical symbol
	auto canonicSym = Symbol::getCanonic (sym);
	// see if there's already a value there
	auto insertRet = table.insert ({canonicSym, atom});
	// there wasn't, so 'atom' was inserted nicely
	if (insertRet.second) {
		return true;
	}
	else {
		// TODO verify variable
		return insertRet.second;
	}
}


AtomPtr Scope::operator[] (const string& sym) {
	auto search = table.find (Symbol::getCanonic (sym));
	return search == table.end () ? move (AtomPtr (nullptr)) : search->second;
}

}
