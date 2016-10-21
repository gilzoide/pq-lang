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

Env::Env () {
	scopeStack.front ().insert ("nil", nullptr);
	// first of all, the Type type ;]
	TypeType = new Type ("Type");
	registerType ("Type", TypeType);
	// native types (ones without heap data)
	IntType = newType ("Int");
	SymbolType = newType ("Symbol");
	RealType = newType ("Real");
}


Env::~Env () {
	delete IntType;
	delete SymbolType;
}


void Env::push (Int value) {
	auto x = pool.requestInt (value);
	atomStack.push_back (x);
}


AtomPtr Env::requestInt (Int value, bool variable) {
	auto ret = pool.requestInt (value, variable);
	ret->updateFatherScope (scopeStack.size ());
	return ret;
}


AtomPtr Env::getLocal (const string& sym) {
	for (auto it = scopeStack.rbegin (); it != scopeStack.rend (); it++) {
		if (auto ret = (*it)[sym]) {
			return ret;
		}
	}

	return nullptr;
}


AtomPtr Env::getGlobal (const string& sym) {
	return scopeStack.front ()[sym];
}


void Env::setLocal (const string& sym, AtomPtr value) {
	value->updateFatherScope (scopeStack.size ());
	scopeStack.back ().insert (sym, value);
}


void Env::setGlobal (const string& sym, AtomPtr value) {
	value->updateFatherScope ((uint16_t) 0);
	scopeStack.front ().insert (sym, value);
}


AtomPtr Env::eval (const Code& code) {
	auto funcSym = code.getFuncSym ();
	auto funcAtom = getLocal (funcSym);
	if (auto func = dynamic_cast<Func *> (funcAtom)) {
		return func->call (*this, code.getArguments ());
	}
	else {
		throw PQ_API_EXCEPTION ("Env::eval", "Symbol \"" + string (funcSym)
				+ "\" ain't a function");
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


Type *Env::newType (const string& name) {
	auto t = new Type (name);
	registerType (name, t);
	return t;
}


Type *Env::newType (const string& name, DataConstructor ctor,
		DataDestructor dtor) {
	auto t = new Type (name, ctor, dtor);
	registerType (name, t);
	return t;
}


void Env::registerType (const string& name, Type *t) {
	auto atom = pool.requestAtom ();
	atom->type = TypeType;
	atom->setValue (t);
	setLocal (name, atom);
}

}
