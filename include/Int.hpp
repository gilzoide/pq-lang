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

using namespace std;

/**
 * Int, the abstract class for PQ integer values
 */
class Int : public Atom {
public:
	/**
	 * Ctor, with value parameter
	 */
	Int (int value);
	/**
	 * Clone method, needed for passing by value semantics
	 *
	 * @note Children must implement this, so our system works
	 */
	virtual Atom *clone () override;

	/**
	 * GETTER for the inner value
	 */
	int getValue ();

	/**
	 * Implicit/explicit conversion to int
	 */
	operator int ();

protected:
	int value;
};

