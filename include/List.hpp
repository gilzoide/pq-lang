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
	/**
	 * Ctor
	 */
	List ();
	/**
	 * Ctor with list parameter
	 *
	 * @note This method's complexity is O(n), as we need the list's last cell
	 *
	 * @param Inner Cons list
	 */
	List (Cons *innerList);

	/**
	 * Clone method override
	 *
	 * @return List clone
	 */
	AtomPtr clone () override;

	/**
	 * GETTER for first element in list
	 */
	AtomPtr getFirst () const;
	/**
	 * GETTER for last element in list
	 */
	AtomPtr getLast () const;

	/**
	 * GETTER for first Cons cell
	 */
	Cons *front () const;
	/**
	 * GETTER for last Cons cell
	 */
	Cons *back () const;

	/**
	 * Append element in the end of the list, creating the necessary Cons cell
	 *
	 * @param elem New element, which will be placed in the end of the list
	 *
	 * @return Newly created Cons cell
	 */
	Cons *append (AtomPtr elem);
	/**
	 * Prepend element as the head of the list, creating the necessary Cons cell
	 *
	 * @param elem New element, which will be placed in the head of the list
	 *
	 * @return Newly created Cons cell
	 */
	Cons *prepend (AtomPtr elem);

	/**
	 * Reset List, making both `first` and `last` as nullptr
	 */
	void reset ();

	/**
	 * Create begin iterator, for iterating in a `for`
	 */
	ConsListIterator begin ();
	/**
	 * Create end iterator (nullptr), for iterating in a `for`
	 */
	ConsListIterator end ();
	
private:
	/// First Cons cell in List, which will have the first element
	Cons *first {nullptr};
	/// Last Cons cell in List, which will have the last element
	Cons *last {nullptr};
};

}
