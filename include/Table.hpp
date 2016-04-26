#pragma once

#include "Symbol.hpp"

#include <unordered_map>
#include <memory>

using namespace std;

/**
 * 'symbol -> Func' map/table
 */
using symTable = unordered_map<symbol, shared_ptr<Atom>>;

/**
 * 'symbol -> Func' Hash Table, used as objects, and as our Scopes
 */
class Table {
public:

private:
	/// The inner symbol/value table
	symTable table;
};
