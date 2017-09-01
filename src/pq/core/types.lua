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

--[[ PQ core types ]]--

local ll = require 'lualvm.llvm'

local Type = require 'pq.type'
local Macro = require 'pq.macro'

--- Register core types in Environment.
return function(env)
	local root = env.scope[1]
	local ctx = env.llvm
	-- The Type type (I'm typing this too much now)
	root.Type = Type
	root.Macro = Macro
	-- Ints
	root.bool = Type.new("i1", ll.Int1TypeInContext(ctx))
	root.i8 = Type.new("i8", ll.Int8TypeInContext(ctx))
	root.i16 = Type.new("i16", ll.Int16TypeInContext(ctx))
	root.i32 = Type.new("i32", ll.Int32TypeInContext(ctx))
	root.i64 = Type.new("i64", ll.Int64TypeInContext(ctx))
	-- The versatile Pointer
	root.Pointer = Type.new("Pointer", ll.PointerType(root.i8.llvm, 0))
end
