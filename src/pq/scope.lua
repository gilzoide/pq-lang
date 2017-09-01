--[[ Copyright © 2016-2017 Gil Barbosa Reis
--
-- This file is part of PQ.
--
-- PQ is free software: you can redistribute it and/or modify
-- it under the terms of the GNU Lesser General Public License as published by
-- the Free Software Foundation, either version 3 of the License, or
-- (at your option) any later version.
--
-- PQ is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU Lesser General Public License for more details.
--
-- You should have received a copy of the GNU Lesser General Public License
-- along with PQ.  If not, see <http://www.gnu.org/licenses/>.
--]]

--[[ PQ Scope, bearer of the imported Modules and definition scoping ]]--


local Scope = {}
Scope.__index = Scope

--- Create a new Scope manager.
function Scope.new()
	return setmetatable({{}}, Scope)
end

--- Push a new scope.
function Scope:push()
	table.insert(self, {})
end

--- Pop a scope.
--
-- Note: you cannot pop the global scope.
function Scope:pop()
	if #self > 1 then
		return table.remove(self)
	end
end

--- Find a symbol in the global scope.
function Scope:get_global(symbol)
	return self[1][symbol]
end

--- Set a symbol locally.
--
-- Note: you cannot unset/reset a symbol, use variables for that.
function Scope:set_local(symbol, val)
	assert(type(symbol) == 'string', "Scope symbol must be a string")
	assert(self[#self][symbol] == nil, "Cannot rebind a symbol in current scope")
	self[#self][symbol] = val
end

--- Find a symbol, starting from local to global scope.
function Scope:get_local(symbol)
	for i = #self, 1, -1 do
		if self[i][symbol] ~= nil then return self[i][symbol] end
	end
	return nil, string.format("Unable to resolve symbol: %q", symbol)
end

return Scope
