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

#include "List.hpp"
#include "Symbol.hpp"
#include "Int.hpp"

namespace pq {

List::List () : List (nullptr) {}


List::List (Cons *innerList) {
	first = innerList;
	// get last Cons cell, if not null
	if (innerList != nullptr) {
		// iterate until last cell (which will have it's `next` as nullptr)
		while (innerList->next != nullptr) {
			innerList = innerList->next;
		}
	}
	last = innerList;
}


List::~List () = default;


AtomPtr List::getFirst () const {
	return first ? first->elem : nullptr;
}


AtomPtr List::getLast () const {
	return last ? last->elem : nullptr;
}


Cons *List::front () const {
	return first;
}


Cons *List::back () const {
	return last;
}


Cons *List::append (AtomPtr elem) {
	// first element in list
	if (last == nullptr) {
		first = last = new Cons (elem);
	}
	else {
		last = last->append (elem);
	}

	return last;
}


Cons *List::append (Cons *cell) {
	// first element in list
	if (last == nullptr) {
		first = last = cell;
	}
	else {
		last = last->append (cell);
	}

	return last;
}


Cons *List::prepend (AtomPtr elem) {
	// first element in list
	if (last == nullptr) {
		first = last = new Cons (elem);
	}
	else {
		first = first->prepend (elem);
	}

	return first;
}


Cons *List::prepend (Cons *cell) {
	// first element in list
	if (last == nullptr) {
		first = last = cell;
	}
	else {
		first = first->prepend (cell);
	}

	return first;
}


Cons *List::reset () {
	// save current Cons list
	auto previousCons = first;
	// and erase it
	first = last = nullptr;

	return previousCons;
}

//----    Iterator    ----//

ConsListIterator List::begin () {
	return ConsListIterator (first);
}


ConsListIterator List::end () {
	return ConsListIterator (nullptr);
}

}
