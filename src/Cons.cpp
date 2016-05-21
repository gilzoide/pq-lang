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

#include "Cons.hpp"

namespace pq {

Cons::Cons () = default;


Cons::Cons (AtomPtr elem, Cons *next) : elem (elem), next (next) {}


Cons::~Cons () = default;


AtomPtr Cons::clone () {
	return new Cons (*this);
}


Cons *Cons::append (AtomPtr elem) {
	auto newCell = new Cons (elem, this->next);
	newCell->updateFatherScope (this);
	this->next = newCell;
	return newCell;
}


Cons *Cons::prepend (AtomPtr elem) {
	auto newCell = new Cons (elem, this);
	newCell->updateFatherScope (this);
	return newCell;
}


void Cons::reset () {
	elem = next = nullptr;
}


ConsListIterator Cons::begin () {
	return ConsListIterator (this);
}


ConsListIterator Cons::end () {
	return ConsListIterator (nullptr);
}


//----    Cons list Iterator    ----//

ConsListIterator::ConsListIterator (Cons *initial) : it (initial) {}


bool ConsListIterator::operator!= (const ConsListIterator& other) const {
	return this->it != other.it;
}


const ConsListIterator& ConsListIterator::operator++ () {
	it = it->next;
	return *this;
}


AtomPtr ConsListIterator::operator* () const {
	return it->elem;
}


}
