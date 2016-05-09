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

namespace pq {

AtomPool::~AtomPool () {
	// delete Ints
	for (auto & i : IntPool) {
		delete i;
	}
}


Int *AtomPool::requestInt () {
	if (!availableInts.empty ()) {
		auto ret = availableInts.top ();
		availableInts.pop ();
		ret->setActive ();
		return ret;
	}
	else {
		auto ret = new Int;
		IntPool.insert (ret);
		return ret;
	}
}


void AtomPool::disposeInt (Int *ptr) {
	// only dispose if ptr is active, as it may have been disposed before
	if (ptr->isActive ()) {
		// clear flags, including ACTIVE, which will be the only one set after
		// a request for an object
		ptr->clearFlags ();
		availableInts.push (ptr);
	}
}

}
