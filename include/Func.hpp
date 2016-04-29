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
 * Callable Functions!
 *
 * Functions that are callable, expecting N (or more) arguments.
 * Any ways of calling these functions are accepted, so we can add
 * functionality to functions, like memoizing, or making them lazy.
 *
 * Function call protocol:
 * 		Receive a List of the arguments passed. If the arguments are not enough,
 * 		returns a proxy of the function with partial application (currying)
 */
class Func : public Atom {
public:
	/**
	 * Clone function override
	 */
	virtual Atom *clone () override;

	/**
	 * getExpectedArgs method override
	 *
	 * @return Number of expected arguments
	 */
	virtual int getExpectedArgs () override;

protected:
	/// How many arguments should we expect?
	int expectedArgs;
};

