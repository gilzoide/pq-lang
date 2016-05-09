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
	CppFunc soma (2, [] (Env& env) {
		auto a = env.getInt (0);
		auto b = env.getInt (1);
		env.pushInt (a + b);
		return 2;
	});

	//e.getLocal ("print");
	//e.call (2);
	//e.call ("print", 2);

	auto list = new Cons (new Int (1), nullptr);
	auto aux = list->append (new Int (2));
	aux = aux->append (new Int (3));
	aux = aux->append (new Int (4));

	for (aux = list; aux; aux = aux->second->as<Cons> ()) {
		cout << aux->first->as<Int> ()->getValue () << ' ';
	}
	cout << endl;

	e.pushInt (1);
	e.pushInt (3);
	soma.call (e);
	cout << e.getInt (0) << endl;

	return 0;
}
