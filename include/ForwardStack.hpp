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

/** @file ForwardStack.hpp
 * A stack based on STL's forward_list
 */
#pragma once

#include <forward_list>

using namespace std;

namespace pq {

/**
 * Stack based on STL's forward_list, for storing the available Ts on the
 * pools
 *
 * std::stack uses much more memory than forward_list, and can do the same.
 * shame on you, STL (I do like STL a lot, but this stack...)
 */
template<typename T>
class ForwardStack {
public:
	/// Push into storage
	void push (const T& value) {
		storage.push_front (value);
		storageSize++;
	}
	/// Push into storage
	void push (T&& value) {
		storage.push_front (value);
		storageSize++;
	}

	/// Pop from storage, returning value
	T pop () {
		T ret = storage.front ();
		storage.pop_front ();
		storageSize--;
		return ret;
	}

	/// Remove a value from list (please don't use it often)
	void remove (const T& value) {
		storage.remove (value);
	}

	/// GETTER for storage size
	size_t size () {
		return storageSize;
	}

	/// Returns if storage is empty
	bool empty () {
		return storageSize == 0;
	}
	
private:
	/// T inner storage
	forward_list<T> storage;
	/// Number of stored elements
	size_t storageSize {0};
};

}
