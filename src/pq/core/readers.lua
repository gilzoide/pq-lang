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

--[[ PQ core readers, the defaults registered at Reader creation ]]--

local ll = require 'lualvm.llvm'

local string_pattern = [['"' { ('\"' / [^"])* } '"']]

local core_readers = {
	--- Quotes.
	{[["'" {.+}]], function(env, atom)
		return atom
	end},
	--- Integer.
	{"{%d+}", function(env, atom)
		return tonumber(atom)
	end},
	--- String.
	{string_pattern, function(env, atom)
		return atom
	end},
	--- String metadata.
	{'"!" ' .. string_pattern, function(env, atom)
		return ll.MDStringInContext(env.llvm, atom)
	end},
}

--- Register core readers in Environment.
return function(env)
	local reader = env.reader
	for _, tuple in ipairs(core_readers) do
		reader:register_pattern(tuple[1], tuple[2])
	end
end
