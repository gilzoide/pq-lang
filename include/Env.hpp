#pragma once

#include "Table.hpp"

#include <vector>

/**
 * PQ environment, with all definitions, scopes...
 */
class Env {
public:

private:
	/**
	 * Our scope stack
	 *
	 * @note This is a vector for easy front (global scope) and back (local
	 * scope) access, and direct scope access (which is nice for debugging)
	 *
	 * Default initialization of global scope
	 */
	vector<Table> scopeStack {1};
};
