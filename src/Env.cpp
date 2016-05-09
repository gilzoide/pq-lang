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

#include "Env.hpp"
#include "Int.hpp"
#include "debug.hpp"

namespace pq {

void Env::pushInt (int value) {
	atomStack.push_back (new Int (value));
}


int Env::getInt (int index) {
	auto ptr = getArg (index)->as<Int> ();
	return ptr->getValue ();
}


AtomPtr Env::getArg (int index) {
	// allow negative indexing
	// @note that out_of_range exception may occur
	if (index < 0) {
		index = atomStack.size () + index;
	}

	// return the raw pointer at index
	return atomStack.at (index);
}


vector<AtomPtr> Env::popArgs (unsigned int number) {
	if (number > atomStack.size ()) {
		throw PQ_API_EXCEPTION ("Env::popArgs", "Not enough arguments to be popped");
	}
	// output vector, already constructed with the values
	// @note that there's no way we access wrong elements, as the size was
	// already checked
	vector<AtomPtr> ret (atomStack.begin (), atomStack.begin () + number);
	// now pop!
	atomStack.erase (atomStack.begin (), atomStack.begin () + number);

	return move (ret);
}

}
