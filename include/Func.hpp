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
#include "Env.hpp"

using namespace std;

namespace pq {

/**
 * Callable Functions!
 *
 * Functions that are callable, expecting N (or more) arguments.
 * Any ways of calling these functions are accepted, so we can add
 * functionality to functions, like memoizing, or making them lazy.
 *
 * Function call protocol:
 * 		Receive the Env and a Cons List of the arguments. If the arguments are
 * 		not enough, returns a proxy of the function with partial application
 * 		(currying)
 */
class Func : public Atom {
public:
	/**
	 * Ctor
	 *
	 * @param Number of expected arguments
	 */
	Func (int numArgs);

	/**
	 * Dtor
	 */
	virtual ~Func ();

	/**
	 * Clone function, which children must implement
	 */
	virtual AtomPtr clone () = 0;

	/**
	 * Method that returns how many arguments are expected by a function
	 *
	 * When the value returned is positive (say, +N), function expects exactly
	 * N arguments. When negative (say, -N), function expects __at least__ N
	 * arguments (more are allowed)
	 *
	 * @return Number of expected arguments
	 */
	virtual int getExpectedArgs ();

	/**
	 * Call function, receiving arguments from Env, and sending results to
	 * the same
	 *
	 * @note This method is _final_ so that it curries the function if there
	 *  wasn't sufficient arguments, calling the virtual Func::_body function with
	 *  Env as parameter, so that it is actually called
	 */
	virtual AtomPtr call (Env& env, Cons *args) final;

protected:
	/// How many arguments should we expect?
	int expectedArgs;

	/**
	 * Proper function body
	 *
	 * This is the function called by @ref call, and it __must__ return how
	 * many arguments should be popped. PQ will automatically do it for its
	 * functions, but you __must__ do it with yours too!
	 */
	virtual AtomPtr body (Env& env, Cons *args) = 0;
};

}
