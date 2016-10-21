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

#include "Type.hpp"

namespace pq {

Type::Type (const string& name) : Type (name, dummyCreateData, dummyDestroyData) {}


Type::Type (const string& name, DataConstructor ctor, DataDestructor dtor)
		: name (Symbol::getCanonic (name)), ctor (ctor), dtor (dtor) {}


Type::~Type () = default;


void Type::constructData (void *& data) {
	ctor (data);
}


void Type::destroyData (void *data) {
	dtor (data);
}


bool Type::is (const string& name) {
	return name == this->name;
}


string Type::getName () {
	return name;
}


void dummyDestroyData (void *) {}


void dummyCreateData (void *&) {}

}
