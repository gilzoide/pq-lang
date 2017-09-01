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

--[[ PQ interpreter environment, holding information for a single session ]]--

local ll = require 'lualvm'

local Reader = require 'pq.reader'
local Scope = require 'pq.scope'
local register_core = require 'pq.core'
local register_prelude = require 'pq.prelude'

local Environment = {}
Environment.__index = Environment

--- Environment destructor: dispose LLVM context.
function Environment:__gc()
	ll.ContextDispose(self.llvm)
end

--- Create a new PQ environment.
function Environment.new()
	local new_env = setmetatable({
		llvm = ll.ContextCreate(),
		reader = Reader.new(),
		scope = Scope.new(),
		compiling = false,
	}, Environment)

	register_core(new_env)
	register_prelude(new_env)

	return new_env
end

--- Evaluate an expression, be it an atom or a S-expression.
function Environment:eval(expr)
	local ty = type(expr)
	if ty == "string" then
		return self.reader:read(self, expr)
	elseif ty == "table" then
		local f = self:eval(expr[1])
		return f(self, select(2, table.unpack(expr)))
	else
		return expr
	end
end

return Environment
