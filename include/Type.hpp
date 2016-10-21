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

/** @file Type.hpp
 * Atom types information
 */
#pragma once

#include "Scope.hpp"
#include "Symbol.hpp"

#include <functional>
#include <string>

namespace pq {

/// Dummy function for destroyData: do nothing
void dummyDestroyData (void *);

/// Dummy function for createData: do nothing
void dummyCreateData (void *&);

/// Prototype for custom data constructors
using DataConstructor = function<void (void *&)>;
/// Prototype for custom data destructors
using DataDestructor = function<void (void *)>;

/**
 * Atom Type, a Scope with data managing
 */
class Type : public Scope {
public:
	/**
	 * Default Ctor, name required
	 */
	Type (const string& name);

	/**
	 * Ctor with data management, both required!
	 *
	 * @param ctor Data constructor
	 * @param dtor Data destructor
	 */
	Type (const string& name, DataConstructor ctor, DataDestructor dtor);
	/**
	 * Dtor
	 */
	virtual ~Type ();

	/**
	 * Construct custom data (or not =P)
	 */
	virtual void constructData (void *& data);
	/**
	 * Destroy custom data (or not =P)
	 */
	virtual void destroyData (void *data);

	/**
	 * Check if type is "name" type
	 */
	virtual bool is (const string& name);

	/**
	 * Get function name
	 */
	string getName ();

protected:
	/// Type name
	symbol name;

private:
	/**
	 * Creates Atom's data, kinda like a constructor
	 */
	DataConstructor ctor {dummyCreateData};
	/**
	 * Destroys Atom's data, kinda like a destructor
	 */
	DataDestructor dtor {dummyDestroyData};
};

}
