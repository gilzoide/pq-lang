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

--[[ PQ parser. Parsing Lisp is not exactly difficult, YAY ]]--

local lpeg = require 'lpeg'

--- Convert nested lists (code) to string, no line ski yet
--
-- @param t The root table containing the S-expression
-- @param res The resulting table, used only on the recursion
--
-- @return Stringified of nested list
local function nestedStr (t, res)
	res = res or {}
	-- Base case: atom
	if type (t) ~= 'table' then
		table.insert (res, tostring (t))
	else
		table.insert (res, '(')
		for _, elem in ipairs (t) do
			nestedStr (elem, res)
		end
		table.insert (res, ')')
	end
	return table.concat (res, ' ')
end

-- Lpeg funcs
local P, S, V = lpeg.P, lpeg.S, lpeg.V
local C, Ct, Cg = lpeg.C, lpeg.Ct, lpeg.Cg

-- Lpeg patterns
local loc = lpeg.locale ()
local space = loc.space
local wspace = space^0
-- local dot = P'.'
-- local quote = P"'"
local symbolStuff = 1 - (space + S"\"\'.()[]{}")
local EOF = P(-1)

local fullpatt = P {
	'S',
	S = Ct (V'sexpr'^0) * EOF,

	sexpr = wspace * '(' * wspace * Ct (V'expr'^0) * ')' * wspace,
	expr = (V'atom' + V'sexpr') * wspace,

	atom = V'quote' + V'number' + V'symbol',
	quote = '\'' * V'expr' / (function (t) return '\'' .. nestedStr (t) end),
	symbol = C (symbolStuff^1),
	-- Numbers
	number = V'hexadecimal' + V'binary' + V'decimal',
	decimal = C (loc.digit^1),
	hexadecimal = '0x' * C (loc.xdigit^1) / '0x%1',
	binary = '0b' * C (S'01'^1) / '0b%1',
}


-- Module
local parser = {}

--- Parse some text
--
-- @param text Text to be parsed
--
-- @return Table with symbols
function parser.parse (text)
	return fullpatt:match (text)
end


--- Parse entire text from file
--
-- @param fileName Input file name
--
-- @return Table with symbols
function parser.parseFile (fileName)
	return parser.parse (io.open (fileName, 'r'):read ('a'))
end


--- Print nested lists, no line skipping yet 
--
-- @param root The root table containing the S-expression
function parser.printNested (root)
	print (nestedStr (root))
end

return parser
