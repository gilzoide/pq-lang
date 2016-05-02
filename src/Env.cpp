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

#include <sstream>

namespace pq {

void Env::pushInt (int value) {
	arguments.push_back (make_shared<Int> (value));
}


int Env::getInt (int index) {
	auto ptr = getArg<Int> (index);
	return (int) *ptr;
}


template<typename T>
T *Env::getArg (int index) {
	// allow negative indexing
	// @note that out_of_range exception may occur
	if (index < 0) {
		index = arguments.size () + index;
	}

	// get the raw pointer at index and try to cast it
	auto ptr = arguments.at (index).get ();
	if (auto castPtr = dynamic_cast<T *> (ptr)) {
		return castPtr;
	}
	else {
		stringstream str;
		str << "Invalid value for conversion to \"" << typeid (T).name () << '"';
		throw runtime_error (str.str ());
	}
}


vector<AtomPtr> Env::popArgs (unsigned int number) {
	if (number > arguments.size ()) {
		throw PQ_API_EXCEPTION ("Env::popArgs", "Not enough arguments to be popped");
	}
	// output vector, already constructed with the values
	// @note that there's no way we access wrong elements, as the size was
	// already checked
	vector<AtomPtr> ret (arguments.begin (), arguments.begin () + number);
	// now pop!
	arguments.erase (arguments.begin (), arguments.begin () + number);

	return move (ret);
}

}
