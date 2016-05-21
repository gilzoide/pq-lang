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

#include "Atom.hpp"

#include <sstream>
#include <algorithm>

namespace pq {

Atom::Atom () = default;


Atom::~Atom () = default;


void Atom::updateFatherScope (uint16_t other) {
	this->fatherScope = min (this->fatherScope, other);
}


void Atom::updateFatherScope (AtomPtr other) {
	updateFatherScope (other->fatherScope);
}


//----    Flags    ----//

void Atom::setFlags (uint8_t flags) {
	this->flags = flags;
}


void Atom::setActive (bool active) {
	if (active) {
		flags |= ACTIVE;
	}
	else {
		flags &= ~ACTIVE;
	}
}


bool Atom::isActive () {
	return flags & ACTIVE;
}


void Atom::setVariable (bool variable) {
	if (variable) {
		flags |= VARIABLE;
	}
	else {
		flags &= ~VARIABLE;
	}
}


bool Atom::isVariable () {
	return flags & VARIABLE;
}


void Atom::setDefined (bool defined) {
	if (defined) {
		flags |= DEFINED;
	}
	else {
		flags &= ~DEFINED;
	}
}


bool Atom::isDefined () {
	return flags & DEFINED;
}


void Atom::clearFlags () {
	flags = 0;
}

}
