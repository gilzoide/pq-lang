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
#include "debug.hpp"

#include <sstream>

namespace pq {

AtomPool::~AtomPool () {
	// delete Ints
	for (auto & i : IntPool) {
		delete i;
	}
	// delete Cons
	for (auto & c : ConsPool) {
		delete c;
	}
	// delete Lists
	for (auto & l : ListPool) {
		delete l;
	}
}


//----    Type request/dispose/delete methods    ----//

//----    Any type (Atom *)    ----//

void AtomPool::track (Atom *ptr) throw (Exception) {
#define checkTypeAndTrack(type) \
	if (auto castPtr = ptr->as<type> ()) \
		track (castPtr)

	checkTypeAndTrack (Int);
	else checkTypeAndTrack (Cons);
	else checkTypeAndTrack (List);
	// type not found: ERROR
	else {
		throw PQ_API_EXCEPTION ("AtomPool::track",
				"Atom type not trackable by Pool");
	}
#undef checkTypeAndtrack
}


void AtomPool::dispose (Atom *ptr) {
#define checkTypeAndDispose(type) \
	if (auto castPtr = ptr->as<type> ()) \
		dispose (castPtr)

	checkTypeAndDispose (Int);
	else checkTypeAndDispose (Cons);
	else checkTypeAndDispose (List);
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
	checkTypeAndDestroy (Cons);
	// type not found: delete pointer!
	else {
		delete ptr;
	}
#undef checkTypeAndDestroy
}


//----    Ints    ----//

Int *AtomPool::requestInt (bool variable) {
	Int *ret;
	if (!availableInts.empty ()) {
		ret = availableInts.pop ();
	}
	else {
		ret = new Int;
		IntPool.insert (ret);
	}
	ret->setActive ();
	ret->setVariable (variable);
	return ret;
}


Int *AtomPool::requestInt (int value, bool variable) {
	auto ret = requestInt (variable);
	ret->setValue (value);
	return ret;
}


void AtomPool::track (Int *ptr) {
	IntPool.insert (ptr);
}


void AtomPool::dispose (Int *ptr) {
	// only dispose if ptr is active, as it may have been disposed before
	if (ptr->isActive ()) {
		// clear flags, including ACTIVE
		ptr->clearFlags ();
		// ensure pointer exists in pool, as one may dispose of an unmanaged
		// pointer
		IntPool.insert (ptr);
		availableInts.push (ptr);
	}
}


void AtomPool::destroy (Int *ptr) {
	// find ptr in availableInts and remove it. This is expensive, dude, think
	// twice
	if (!ptr->isActive ()) {
		availableInts.remove (ptr);
	}
	// erase ptr from the pool
	IntPool.erase (ptr);
	delete ptr;
}


//----    Cons    ----//

Cons *AtomPool::requestCons (bool variable) {
	Cons *ret;
	if (!availableCons.empty ()) {
		ret = availableCons.pop ();
		ret->elem = ret->next = nullptr;
	}
	else {
		ret = new Cons;
		ConsPool.insert (ret);
	}
	ret->setActive ();
	ret->setVariable (variable);
	return ret;
}


Cons *AtomPool::requestCons (AtomPtr elem, bool variable) {
	auto ret = requestCons (variable);
	ret->elem = elem;
	return ret;
}


void AtomPool::track (Cons *ptr) {
	ConsPool.insert (ptr);
}


void AtomPool::dispose (Cons *ptr) {
	// only dispose if ptr is active, as it may have been disposed before
	if (ptr && ptr->isActive ()) {
		// clear flags, including ACTIVE
		ptr->clearFlags ();
		// ensure pointer exists in pool, as one may dispose of an unmanaged
		// pointer
		ConsPool.insert (ptr);
		availableCons.push (ptr);

		// also dispose of it's elements, please
		dispose (ptr->elem);
		dispose (ptr->next);
	}
}


void AtomPool::destroy (Cons *ptr) {
	// find ptr in availableCons and remove it. This is expensive, dude, think
	// twice
	if (!ptr->isActive ()) {
		availableCons.remove (ptr);
	}
	// erase ptr from the pool
	ConsPool.erase (ptr);
	delete ptr;
}


//----    List    ----//

List *AtomPool::requestList () {
	List *ret;
	if (!availableList.empty ()) {
		ret = availableList.pop ();
		ret->reset ();
	}
	else {
		ret = new List;
		ListPool.insert (ret);
	}
	ret->setActive ();
	return ret;

}


void AtomPool::track (List *ptr) {
	ListPool.insert (ptr);
}


void AtomPool::dispose (List *ptr) {
	// only dispose if ptr is active, as it may have been disposed before
	if (ptr->isActive ()) {
		// clear flags, including ACTIVE
		ptr->clearFlags ();
		// ensure pointer exists in pool, as one may dispose of an unmanaged
		// pointer
		ListPool.insert (ptr);
		availableList.push (ptr);

		// also dispose of it's Cons list
		dispose (ptr->front ());
	}
}


//----    nullptr    ----//

void AtomPool::dispose (nullptr_t ptr) {/*yup, actually don't do anything =P */}


//----    End of Type specific methods    ----//


string AtomPool::getMemoryStats () {
	ostringstream os;
	// Int
	auto totalInts = IntPool.size ();
	os << "Int - total: " << totalInts << "; Inactive: " <<
			availableInts.size () << "; 1xRAM: " << sizeof (Int) << "B sum = "
		   	<< totalInts * sizeof (Int) << 'B' << endl;
	// Cons
	auto totalCons = ConsPool.size ();
	os << "Cons - total: " << totalCons << "; Inactive: " <<
			availableCons.size () << "; 1xRAM: " << sizeof (Cons) << "B sum = "
		   	<< totalCons * sizeof (Cons) << 'B' << endl;

	return os.str ();
}

}
