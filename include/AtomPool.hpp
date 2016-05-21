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

/** @file AtomPool.hpp
 * Atom pool, an Object Pool for PQ Atoms
 */
#pragma once

#include "Atom.hpp"
#include "ForwardStack.hpp"
#include "Int.hpp"
#include "Symbol.hpp"
#include "Table.hpp"
#include "Cons.hpp"
#include "List.hpp"
#include "Exception.hpp"

#include <unordered_set>

namespace pq {

/**
 * A Object pool for PQ Atoms
 *
 * Every Env has an AtomPool like this one. That said, whenever you pass an Atom
 * to an Env, it gets to manage the Atom's memory, so you __must not__ free it
 * yourself ^^
 */
class AtomPool {
public:
	/**
	 * Dtor, actually deletes every object hold
	 */
	~AtomPool ();

	//----    Type request/track/dispose/delete methods    ----//
	
	//----    Any type (Atom *)    ----//
	/**
	 * Keep track (insert in pool) of Atom pointer, letting AtomPool manage its
	 * memory
	 *
	 * If ptr is not a type pooled by AtomPool, this function throws Exception
	 *
	 * @warning By letting AtomPool track your pointer, one __must not__ delete
	 * the pointer by hand, avoiding 'double free'
	 *
	 * @param ptr Pointer to be tracked
	 *
	 * @throw Exception if ptr is not of a trackable Atom type
	 */
	void track (Atom *ptr) throw (Exception);
	/**
	 * Delete an Atom, taking care of if it is a pooled Atom type
	 */
	void destroy (Atom *ptr);
	/**
	 * Dispose of any Atom pointer type
	 *
	 * This method claims memory of any Atom pointer type. Some of them should
	 * be placed in the pools (Int, Symbol, Strings, Cons, List...), so
	 * __all__ of them are tested before deleting the pointer.
	 *
	 * @note If you are sure of which type your pointer is, please use the
	 * corresponding overload as it's faster. It is safe to call
	 * `AtomPool.dispose (atomPtr->as<some_Atom_type> ())` even if the result is
	 * `nullptr`, as there is an overload for that ;]
	 *
	 * @warning Don't ever use 'ptr' after disposing of it, even if you know
	 * it's type and know it will be pooled
	 *
	 * @param ptr Atom pointer to be disposed
	 */
	void dispose (Atom *ptr);

	//----    Ints    ----//
	/**
	 * Returns a usable Int
	 *
	 * If there are disposed Ints, this function will pop the first one,
	 * activate and return it. Otherwise, it creates a new one and returns it.
	 *
	 * @note ACTIVE flag will __always__ be on
	 *
	 * @param variable Set Int's VARIABLE flag?
	 *
	 * @return Usable Int
	 */
	Int *requestInt (bool variable = false);
	/**
	 * Returns a usable Int with value `value`
	 */
	Int *requestInt (int value, bool variable = false);
	/**
	 * Track Int, adding it to Int pool
	 *
	 * @note If Int is already tracked, doesn't do a thing
	 *
	 * @param ptr Pointer to be tracked
	 */
	void track (Int *ptr);
	/**
	 * Dispose of an Int, returning it to the pool
	 *
	 * @param ptr Int pointer to be disposed
	 */
	void dispose (Int *ptr);
	/**
	 * Delete an Int, taking it out of the pool and actually deleting it's memory
	 *
	 * @note If ptr is an inactive Int (`ptr->isActive () == false`), it will
	 * have to be searched for and erased from availableInts. TL;DR: avoid
	 * `AtomPool.destroy (inactiveIntPtr);` =P
	 *
	 * @param ptr Int pointer that will be OBLITERATED
	 */
	void destroy (Int *ptr);

	//----    Cons    ----//
	/**
	 * Returns a usable Cons
	 *
	 * If there are disposed Cons, this function will pop the first one,
	 * activate and return it. Otherwise, it creates a new one and returns it.
	 *
	 * @note ACTIVE flag will __always__ be on
	 *
	 * @param variable Set Cons' VARIABLE flag?
	 *
	 * @return Usable Cons
	 */
	Cons *requestCons (bool variable = false);
	/**
	 * Returns a usable Cons with element `elem`
	 *
	 * @param elem Element inserted into returned Cons
	 * @param variable Set Cons' VARIABLE flag?
	 *
	 * @return Usable Cons
	 */
	Cons *requestCons (AtomPtr elem, bool variable = false);
	/**
	 * Track Cons, adding it to Cons pool
	 *
	 * @note If Cons is already tracked, doesn't do a thing
	 *
	 * @param ptr Pointer to be tracked
	 */
	void track (Cons *ptr);
	/**
	 * Dispose of an Cons, returning it to the pool
	 *
	 * @param ptr Cons pointer to be disposed
	 */
	void dispose (Cons *ptr);
	/**
	 * Delete a Cons, taking it out of the pool and actually deleting it's memory
	 *
	 * @note If ptr is an inactive Cons (`ptr->isActive () == false`), it will
	 * have to be searched for and erased from availableCons. TL;DR: avoid
	 * `AtomPool.destroy (inactiveConsPtr);` =P
	 *
	 * @param ptr Int pointer that will be OBLITERATED
	 */
	void destroy (Cons *ptr);

	//----    List    ----//
	/**
	 * Returns a usable List
	 *
	 * @return Usable List
	 */
	List *requestList ();
	/**
	 * Track List, adding it to List pool
	 *
	 * @note If List is already tracked, doesn't do a thing
	 *
	 * @param ptr Pointer to be tracked
	 */
	void track (List *ptr);
	/**
	 * Dispose of a List, returning it to the pool
	 *
	 * @param ptr List pointer to be disposed
	 */
	void dispose (List *ptr);

	//----    nullptr    ----//
	/**
	 * Dispose overload for `nullptr`
	 *
	 * This is defined so that one can call `AtomPool.dispose (atomPtr->as<some_Atom_type> ())`
	 * and it doesn't put the pointer in the wrong place (if it was not of the
	 * right type).
	 *
	 * It actually doesn't do anything. This isn't necessarily bad, as memory
	 * will be claimed after AtomPool's destructor (AtomPool::~AtomPool).
	 *
	 * @note If you are not sure which type your pointer is and really want
	 * memory to be claimed, use the 'Atom *' overload instead
	 *
	 * @param ptr A `nullptr`
	 */
	void dispose (nullptr_t ptr);

	//----    End of Type specific methods    ----//

	/**
	 * Returns a string with the memory stats, including how many objects are
	 * instantiated and how many are inactive
	 */
	string getMemoryStats ();

private:
	/// Int Pool
	unordered_set<Int *> IntPool;
	/// Stack of available (inactive) Ints
	ForwardStack<Int *> availableInts;

	/// Cons Pool
	unordered_set<Cons *> ConsPool;
	/// Stack of available (inactive) Cons
	ForwardStack<Cons *> availableCons;

	/// Cons Pool
	unordered_set<List *> ListPool;
	/// Stack of available (inactive) List
	ForwardStack<List *> availableList;
};

}
