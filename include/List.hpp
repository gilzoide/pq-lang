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

#pragma once

#include "Atom.hpp"
#include "Cons.hpp"

namespace pq {

/**
 * List, a singly linked list with front/back access and easy insertion/removal,
 * constructed from Cons structures
 */
class List : public Atom {
public:
	
private:
	/// First Cons cell in List, which will have the first element
	Cons *front {nullptr};
	/// Last Cons cell in List, which will have the last element
	Cons *back {nullptr};
};

}
