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
#include "Type.hpp"
#include "debug.hpp"

#include <sstream>

namespace pq {

AtomPool::~AtomPool () {
	// delete Atoms
	for (auto & a : pool) {
		delete a;
	}
	// delete Cons
	for (auto & c : ConsPool) {
		delete c;
	}
}


//----    Type request/dispose/delete methods    ----//

//----    Any type (Atom *)    ----//

void AtomPool::track (Atom *ptr) {
	pool.insert (ptr);		
}


Atom *AtomPool::requestAtom () {
	Atom *ret;
	if (!availableAtoms.empty ()) {
		ret = availableAtoms.pop ();
	}
	else {
		ret = new Atom;
		pool.insert (ret);
	}
	ret->setActive ();
	return ret;
}


void AtomPool::dispose (Atom *ptr) {
	// only dispose if ptr is active, as it may have been disposed before
	if (ptr && ptr->isActive ()) {
		// clear flags, including ACTIVE
		ptr->clearFlags ();
		// ensure pointer exists in pool, as one may dispose of an unmanaged
		// pointer
		pool.insert (ptr);
		availableAtoms.push (ptr);
	}
}


void AtomPool::destroy (Atom *ptr) {
	// find ptr in availableAtoms and remove it. This is expensive, dude, think
	// twice
	if (!ptr->isActive ()) {
		availableAtoms.remove (ptr);
	}
	// erase ptr from the pool
	pool.erase (ptr);
	delete ptr;
}


//----    Ints    ----//

Atom *AtomPool::requestInt (bool variable) {
	Atom *ret;
	if (!availableAtoms.empty ()) {
		ret = availableAtoms.pop ();
	}
	else {
		ret = new Atom;
		pool.insert (ret);
	}
	ret->setActive ();
	ret->setVariable (variable);
	return ret;
}


Atom *AtomPool::requestInt (Int value, bool variable) {
	auto ret = requestInt (variable);
	ret->setValue (value);
	return ret;
}


//----    Cons    ----//

Cons *AtomPool::requestCons () {
	Cons *ret;
	if (!availableCons.empty ()) {
		ret = availableCons.pop ();
		ret->reset ();
	}
	else {
		ret = new Cons;
		ConsPool.insert (ret);
	}
	return ret;
}


Cons *AtomPool::requestCons (AtomPtr elem) {
	auto ret = requestCons ();
	ret->elem = elem;
	return ret;
}


void AtomPool::track (Cons *ptr) {
	ConsPool.insert (ptr);
}


void AtomPool::dispose (Cons *ptr) {
	// only dispose if ptr is active, as it may have been disposed before
	if (ptr) {
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
	availableCons.remove (ptr);

	// erase ptr from the pool
	ConsPool.erase (ptr);
	delete ptr;
}

//----    End of Type specific methods    ----//


string AtomPool::getMemoryStats () {
	ostringstream os;
	size_t total;
	size_t atomSize;

	total = pool.size ();
	atomSize = sizeof (Atom);
	os << "Atom\t total: " << total << "\t Inactive: " << availableAtoms.size ()
			<< "\t each = " << atomSize << "B, sum = " << total * atomSize << 'B' << endl;


#define printMemory(type) \
	total = type ## Pool.size (); \
	atomSize = sizeof (type); \
	os << #type "\t total: " << total << "\t Inactive: " << available ## type.size () \
			<< "\t each = " << atomSize << "B, sum = " << total * atomSize << 'B' << endl

	printMemory (Cons);
#undef printMemory

	return os.str ();
}

}
