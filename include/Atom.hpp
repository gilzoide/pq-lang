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

/** @file Atom.hpp
 * Atom, abstraction for all PQ values
 */
#pragma once

#include "Exception.hpp"
#include "Int.hpp"
#include "Real.hpp"
#include "Symbol.hpp"
#include "Type.hpp"
#include "debug.hpp"

#include <memory>
#include <sstream>
#include <type_traits>

using namespace std;

namespace pq {

class Atom;
class List;
class Env;

/// Atom pointer, used in PQ
using AtomPtr = Atom *;

/// Atom possible flags, OR them to apply more than one
enum AtomFlags : uint8_t {
	ACTIVE = 0x01,
	VARIABLE = 0x02,
	DEFINED = 0x04,
	SUBLIST = 0x08,
};

/**
 * Atom, the abstract class for PQ values
 *
 * Everything is a function in PQ, so we use this OO to help everything work
 * smooth and easily
 */
class Atom {
	/// Env can mess with atom "à vonts"
	friend class Env;
public:
	/**
	 * Default Ctor, no type, no value, useless in this state
	 */
	Atom ();
	/**
	 * Dtor, calls Type destruction on value
	 */
	virtual ~Atom ();

	/**
	 * Clone method, needed for passing by value semantics
	 *
	 * @note Children must implement this, so our system works
	 *
	 * @return Atom clone
	 */
	AtomPtr clone ();

	/**
	 * GETTER for @ref fatherScope
	 *
	 * @return fatherScope
	 */
	uint16_t getFatherScope ();
	/**
	 * Update `this->fatherScope` with `other` if needed
	 *
	 * fatherScope should (and will) be the one with minimum index between
	 * `this` and `other`. If `this->fatherScope` is less than `other` one,
	 * just ignore it
	 *
	 * @param other Other fatherScope index
	 */
	void updateFatherScope (uint16_t other);
	/**
	 * Update fatherScope based on another Atom's one
	 *
	 * @param other Other AtomPtr, for reference
	 */
	void updateFatherScope (AtomPtr other);

	//----    Value SETTERS   ----//
	/**
	 * Set value as Integer
	 *
	 * @param i New value as an Integer
	 */
	void setValue (Int i);
	/**
	 * Set value as Symbol
	 *
	 * @param sym New value as a symbol
	 */
	void setValue (symbol sym);
	/**
	 * Set value as Type
	 *
	 * @param type New value as a Type
	 */
	void setValue (Type *type);
	/**
	 * Set value as custom data
	 *
	 * Atom's Type must have the corresponding @ref destroyData to destroy this
	 * pointer
	 *
	 * @param customData New value as custom data, as a void *
	 */
	void setValue (void *customData);

	//----    Value GETTERS    ----//
	/**
	 * Get Integer method that throws exceptions if Atom ain't Int
	 */
	Int asInt () throw (Exception);
	/**
	 * Get symbol method that throws exceptions if Atom ain't Symbol
	 */
	symbol asSymbol () throw (Exception);

	/**
	 * Get custom data pointer method that throws exception if Atom ain't of type
	 */
	template<typename T>
	T *asData () throw (Exception) {
		return dynamic_cast<T *> (value.data);
	}

	//----    Type    ----//
	/**
	 * GETTER for the type
	 */
	Type *getType ();

	//----    Flags    ----//
	/**
	 * SETTER for flags, all of them
	 *
	 * Use bit-or `|` or `+` to concat wanted flags
	 *
	 * @param flags New flags
	 *
	 * @sa AtomFlags
	 */
	void setFlags (uint8_t flags);
	/**
	 * SETTER for ACTIVE flag
	 *
	 * @param active New ACTIVE flag value
	 */
	void setActive (bool active = true);
	/**
	 * GETTER for ACTIVE flag
	 *
	 * @return ACTIVE flag value
	 */
	bool isActive ();
	/**
	 * SETTER for VARIABLE flag
	 *
	 * @param active New VARIABLE flag value
	 */
	void setVariable (bool variable = true);
	/**
	 * GETTER for VARIABLE flag
	 *
	 * @return VARIABLE flag value
	 */
	bool isVariable ();
	/**
	 * SETTER for DEFINED flag
	 *
	 * @param active New DEFINED flag value
	 */
	void setDefined (bool defined = true);
	/**
	 * GETTER for DEFINED flag
	 *
	 * @return DEFINED flag value
	 */
	bool isDefined ();

	/**
	 * Set every flag as false, somewhat resetting Atom
	 */
	void clearFlags ();

	/// Constant that means an Atom is not defined in any scope
	constexpr static uint16_t notScoped = (uint16_t) -1;

protected:
	/// Atom's type
	Type *type;

	/// Atom value, which may be a simple/builtin one, or anything else you
	/// like / need
	union {
		Int i;
		Real d;
		symbol sym;
		List *list;
		Type *type;
		void *data;
	} value;

	/**
	 * Index of which scope owns this Atom
	 *
	 * This is used for memory management, so that Atoms get released as the
	 * scope dies, as described in [Memory](design/memoria.md)
	 *
	 * @note Default value is @ref notScoped, so that if you don't set Atom's
	 * fatherScope, any other managed Atom will dispose this object immediately
	 */
	uint16_t fatherScope {notScoped};

	/**
	 * Flags, 8 bits is better than 8 bytes (bitset)
	 *
	 * - Active: is Atom active? (is it live, or is it waiting for a new
	 *   oportunity in the AtomPoll?
	 * - Variable: is it variable (or immutable) ?
	 * - Defined: is Atom defined / does it have a value?
	 *
	 * Please, use the methods rather than this variable directly
	 *
	 * @sa AtomFlags
	 */
	uint8_t flags {ACTIVE};
};

}
