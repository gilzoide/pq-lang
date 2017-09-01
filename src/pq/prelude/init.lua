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

--[[ PQ prelude: stuff we can't live without (except for sandboxes) ]]--

local Macro = require 'pq.macro'

return function(env)
	env.scope[1].let = Macro.new(function(env, symbol, val)
		val = env:eval(val)
		env.scope:set_local(symbol, val)
		return val
	end)
end
