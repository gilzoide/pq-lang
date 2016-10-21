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

#include "Scope.hpp"
#include "AtomPool.hpp"
#include "Type.hpp"
#include "Code.hpp"

#include <vector>

namespace pq {

/**
 * PQ environment, with all definitions, scopes...
 */
class Env {
public:
	/**
	 * Ctor, initializes all
	 */
	Env ();
	/**
	 * Dtor
	 */
	~Env ();

	/**
	 * Push an integer to the Atom stack
	 *
	 * @param value Int value
	 */
	void push (Int value);

	/**
	 * Request an Int from pool
	 *
	 * @sa AtomPool::requestInt
	 */
	AtomPtr requestInt (Int value, bool variable = false);

	/**
	 * Get local Atom associated with symbol `sym`
	 *
	 * Search will be made from last to first scope. Remember that `nullptr` is
	 * a valid Atom, with `nil` value
	 *
	 * @param sym Symbol to be searched
	 *
	 * @return Atom associated with `sym`
	 */
	AtomPtr getLocal (const string& sym);
	/**
	 * Get global Atom associated with symbol `sym`
	 *
	 * Search will be made only on first scope. Remember that `nullptr` is
	 * a valid Atom, with `nil` value
	 *
	 * @param sym Symbol to be searched
	 *
	 * @return Atom associated with `sym`
	 */
	AtomPtr getGlobal (const string& sym);

	/**
	 * Set a local symbol to value `value`
	 *
	 * @param sym Symbol to be associated
	 * @param value Value to be associated to `sym`
	 */
	void setLocal (const string& sym, AtomPtr value);
	/**
	 * Set a global symbol to value `value`
	 *
	 * @param sym Symbol to be associated
	 * @param value Value to be associated to `sym`
	 */
	void setGlobal (const string& sym, AtomPtr value);

	/**
	 * Pop an argument value
	 *
	 * @note Args will be modified by this method, being advanced to its `next`
	 *
	 * @param args Cons list of arguments, used in a stack way
	 *
	 * @return Argument value
	 */
	AtomPtr popArg (Cons *& args);

	/**
	 * Evaluate a Code, calling its Func with provided arguments
	 *
	 * @param code Code to be evaluated
	 *
	 * @return Function return
	 *
	 * @throw pq::Exception if first symbol isn't associated to a function
	 */
	AtomPtr eval (const Code& code);

	/**
	 * Create a new type and associate it in the local scope
	 */
	Type *newType (const string& name);
	/**
	 * Create a new type with custom data and associate it in the local scope
	 */
	Type *newType (const string& name, DataConstructor ctor,
			DataDestructor dtor);

private:
	/**
	 * Stack that mantains Atoms for function calling, function return 
	 */
	vector<AtomPtr> atomStack;

	/**
	 * Our scope stack
	 *
	 * @note This is a vector for easy front (global scope) and back (local
	 * scope) access, and direct scope access (which is nice for debugging)
	 *
	 * Default initialization of global scope
	 */
	vector<Scope> scopeStack {1};

	/**
	 * AtomPool, for object and memory managing
	 */
	AtomPool pool;

	//----    Builtin Types    ----//
	Type *TypeType;
	Type *IntType;
	Type *SymbolType;
	Type *RealType;

	/// Auxiliary function to register newTypes in the local scope
	void registerType (const string& name, Type *t);
};

}
