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

--[[ PQ atom reader: atom -> value ]]--

local re = require 'relabel'

local Reader = {}
Reader.__index = Reader

--- Create a new Reader.
function Reader.new()
	return setmetatable({}, Reader)
end

--- Register a new pattern for the reader with a function that returns the wanted
-- value.
function Reader:register_pattern(patt, f)
	assert(type(patt) == "string", "Pattern for reader must be a string")
	local compiled = assert(re.compile(patt .. " !."))
	self[#self + 1] = {compiled, f}
end

--- Read an atom, returning the value produced by the registered function.
function Reader:read(env, atom)
	for _, tuple in ipairs(self) do
		local m = tuple[1]:match(atom)
		if m then
			return tuple[2](env, m)
		end
	end
	return assert(env.scope:get_local(atom))
end

return Reader
