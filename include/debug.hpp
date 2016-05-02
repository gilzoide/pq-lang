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

#include "Exception.hpp"

#include <sstream>

/**
 * API exception, that tells us where problem occurred and why
 */
#define PQ_API_EXCEPTION(funcName, what) \
	[&] () { \
		ostringstream os; \
		os << "[pq::" << funcName << " @ " << __FILE__ << ':' << __LINE__ \
				<< "] " << what; \
		return move (Exception (move (os.str ()))); \
	} ();
