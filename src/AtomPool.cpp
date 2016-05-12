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

#include "AtomPool.hpp"

#include <sstream>

namespace pq {

AtomPool::~AtomPool () {
	// delete Ints
	for (auto & i : IntPool) {
		delete i;
	}
}


//----    Type get/dispose/delete methods    ----//

//----    Any type (Atom *)    ----//

void AtomPool::dispose (Atom *ptr) {
#define checkTypeAndDispose(type) \
	if (auto castPtr = ptr->as<type> ()) \
		dispose (castPtr)

	checkTypeAndDispose (Int);
	// type not found: delete pointer!
	else {
		delete ptr;
	}
#undef checkTypeAndDispose
}


void AtomPool::destroy (Atom *ptr) {
#define checkTypeAndDestroy(type) \
	if (auto castPtr = ptr->as<type> ()) \
		destroy (castPtr)

	checkTypeAndDestroy (Int);
	// type not found: delete pointer!
	else {
		delete ptr;
	}
}


//----    Ints    ----//

Int *AtomPool::requestInt () {
	if (!availableInts.empty ()) {
		auto ret = availableInts.pop ();
		ret->setActive ();
		return ret;
	}
	else {
		auto ret = new Int;
		IntPool.insert (ret);
		return ret;
	}
}


void AtomPool::dispose (Int *ptr) {
	// only dispose if ptr is active, as it may have been disposed before
	if (ptr->isActive ()) {
		// clear flags, including ACTIVE, which will be the only one set after
		// a request for an object
		ptr->clearFlags ();
		availableInts.push (ptr);
	}
}


void AtomPool::destroy (Int *ptr) {
	// find ptr in availableInts and remove it. This is expensive, dude, think
	// twice. I'll
	if (!ptr->isActive ()) {
		availableInts.remove (ptr);
	}
	// erase ptr from the pool
	IntPool.erase (ptr);
	delete ptr;
}


//----    End of Type specific methods    ----//

void AtomPool::dispose (nullptr_t ptr) {/*yup, actually don't do anything =P */}


string AtomPool::getMemoryStats () {
	ostringstream os;
	os << "Int - total: " << IntPool.size () << "; Inactive: " <<
			availableInts.size () << endl;

	return os.str ();
}

}
