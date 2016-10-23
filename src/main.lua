#!/usr/bin/env lua
--[[ Copyright © 2016 Gil Barbosa Reis
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

--[[ PQ interpreter executable main file ]]--

local parser = require 'pq.parser'

parser.printNested (assert (parser.parseFile (assert (arg[1], 'Favor, dê-me um arquivo pra parsear'))))
