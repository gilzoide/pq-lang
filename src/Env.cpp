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

#include "Env.hpp"
#include "Func.hpp"
#include "debug.hpp"

namespace pq {

void Env::push (int value) {
	auto x = pool.requestInt (value);
	atomStack.push_back (x);
}


int Env::getInt (int index) {
	auto ptr = getArg (index)->assert<Int> ();
	return ptr->getValue ();
}


AtomPtr Env::getArg (int index) {
	// allow negative indexing
	// @note that out_of_range exception may occur
	if (index < 0) {
		index = atomStack.size () + index;
	}

	// return the raw pointer at index
	return atomStack.at (index);
}


AtomPtr Env::getLocal (const string& sym) {
	for (auto it = scopeStack.rbegin (); it != scopeStack.rend (); it++) {
		if (auto ret = (*it)[sym]) {
			return ret;
		}
	}

	return nullptr;
}


void Env::setLocal (const string& sym, AtomPtr value) {
	value->updateFatherScope (scopeStack.size ());
	scopeStack.back ().insert (sym, value);
}


AtomPtr Env::eval (const Code& code) {
	string funcSym = code.getFuncSym ()->getSym ();
	auto func = getLocal (funcSym)->as<Func> ();
	if (func) {
		return func->call (*this, code.getArguments ());
	}
	else {
		throw PQ_API_EXCEPTION ("Env::eval", "Symbol \"" + funcSym + "\" ain't a function");
	}
}


AtomPtr Env::popArg (Cons *& args) {
	if (args) {
		auto ret = args->elem;
		// save first Cons cell, so we can dispose of it
		auto garbage = args;
		// advance args
		args = args->next;

		// reset first, so that we don't yet dispose of `ret` nor `args`
		garbage->reset ();
		pool.dispose (garbage);

		return ret;
	}
	else {
		throw PQ_API_EXCEPTION ("Env::popArg",
				"Empty argument list. Maybe you popped more args than you asked for?");
	}
}


vector<AtomPtr> Env::popArgs (unsigned int number) {
	if (number > atomStack.size ()) {
		throw PQ_API_EXCEPTION ("Env::popArgs", "Not enough arguments to be popped");
	}
	// output vector, already constructed with the values
	// @note that there's no way we access wrong elements, as the size was
	// already checked
	vector<AtomPtr> ret (atomStack.begin (), atomStack.begin () + number);
	// now pop!
	atomStack.erase (atomStack.begin (), atomStack.begin () + number);

	return move (ret);
}

}
