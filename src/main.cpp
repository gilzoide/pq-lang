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

#include "pq.hpp"

#include <iostream>

using namespace std;
using namespace pq;

int main (int argc, char **argv) {
	Env e;
	CppFunc soma (2, [] (Env& env, Cons *args) {
		auto a = env.popArg (args)->assert<Int> ();
		auto b = env.popArg (args)->assert<Int> ();
		return new Int (a->getValue () + b->getValue ());
	});

	e.setLocal ("printa", new CppFunc (1, [] (Env& env, Cons *args) {
		auto s = env.popArg (args)->assert<Symbol> ();
		cout << "[printa] " << s->getSym () << endl;
		return nullptr;
	}));

	//e.getLocal ("print");
	//e.call (2);
	//e.call ("print", 2);

	AtomPool pool;

	Code C;
	C << "printa" << "Como vão vocês?";
	e.eval (C);

	List list;
	list.append (pool.requestInt (1));
	list.append (pool.requestInt (2));
	list.prepend (pool.requestInt (3));
	list.append (pool.requestInt (4));

	for (auto it : list) {
		cout << it->assert<Int> ()->getValue () << ' ';
	}
	cout << endl;

	//e.pushInt (1);
	//e.pushInt (3);
	//soma.call (e);
	//cout << e.getInt (0) << endl;
	
	auto i = pool.requestInt (true);
	i->setValue (3);
	i->setValue (4);

	cout << i << " -> " << (int) *i << endl;

	i = pool.requestInt ();

	cout << i << " -> " << i->getValue () << endl;

	pool.dispose (i);
	cout << pool.getMemoryStats ();

	return 0;
}
