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

#include "Func.hpp"

#include <functional>

using namespace std;

namespace pq {

/**
 * C++ functions, registered by any means std::function accepts
 */
class CppFunc : public Func {
public:
	/**
	 * Ctor, must give the function, and number of expected arguments
	 */
	CppFunc (int numArgs, function<int (Env&)> f);

	/**
	 * Clone function override
	 */
	virtual Atom *clone () override;

protected:
	/**
	 * Calls the internal _body function
	 */
	virtual int body (Env& env) override;

	/// Actual function
	function<int (Env&)> _body;
};

}
