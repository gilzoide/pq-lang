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

local lpeg = require 'lpeglabel'
local re = require 'relabel'

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
local P, S, V, R, B = lpeg.P, lpeg.S, lpeg.V, lpeg.R, lpeg.B
local C, Ct, Cg = lpeg.C, lpeg.Ct, lpeg.Cg
local T = lpeg.T

-- Lpeg patterns
local loc = lpeg.locale ()
local space = loc.space
local symbolStuff = 1 - (space + S"\"\'.;()[]{}")
local EOL = P'\n'
local EOF = P(-1)
local wspace = space^0 * (';' * (1 - EOL)^0 * EOL)^-1

-- LpegLabel errors
local parseErrors = {}
-- Regular PEG error
parseErrors[0] = 'PEG não conseguiu parsear'
local function addError (label, msg)
	table.insert (parseErrors, msg)
	parseErrors[label] = #parseErrors
end
addError ('ParenErr', 'falta parênteses')
addError ('MuchParenErr', 'tem parênteses sobrando')
addError ('StringErr', 'string não fechada')
addError ('BinErr', 'números binários só podem ser formados por 0 e 1')
addError ('OctErr', 'números octais só podem ser formados por 0-7')
addError ('HexErr', 'números hexadecimais só podem ser formados por 0-9, a-f e A-F')
addError ('CharErr', 'caractere não reconhecido como inicial')

local function expect (patt, err)
	local label = assert (parseErrors[err], 'Erro "' .. err .. '" não declarado')
	return patt + T (label)
end

local fullpatt = P {
	'S',
	S = V'shebang'^-1 * wspace * Ct ((V'sexpr' * wspace)^0) * EOF,
	shebang = '#!' * (1 - EOL)^1 * EOL,

	sexpr = '(' * wspace * Ct (V'exprs') * wspace * expect (')', 'ParenErr'),
	exprs = V'expr' * (space^1 * V'expr')^0 + P(0),
	expr = V'sexpr' + V'atom',

	atom = V'quote' + V'number' + V'string' + V'keyword' + V'symbol',
	-- Symbolic stuff
	quote = C ('\'' * V'expr') / 1,
	keyword = C (':' * V'symbol'),
	symbol = C (symbolStuff^1),
	-- String
	string = C ('"' * (P'\\"' + 1 - S'\n"')^0 * expect ('"', 'StringErr')),
	-- Numbers
	number = C (V'hexadecimal' + V'octal' + V'binary' + V'decimal'),
	decimal = loc.digit^1,
	octal = '0o' * expect (R'07'^1, 'OctErr'),
	hexadecimal = '0x' * expect (loc.xdigit^1, 'HexErr'),
	binary = '0b' * expect (S'01'^1, 'BinErr'),
}

re.setlabels (parseErrors)
local g = re.compile ([[
S		<- Shebang? Sp {| (Sexpr Sp)* |} (!. / %{MuchParenErr})
Shebang	<- '#!' AteEOL

Sexpr	<- '(' Sp -> "nil" ')' / '(' Sp {| Exprs |} Sp (')' / %{ParenErr})
Exprs	<- Expr (%s+ Expr)*
Expr	<- Sexpr / Atom

-- Atomos
Atom	<- Quote / Number / String / Keyword / Symbol / Sp / %{CharErr}
Quote	<- { "'" Expr } -> 1
Symbol	<- { SymChar+ }
SymChar	<- [^]["'\;(){}%s] -- Símbolos não podem com essas coisa especial
String	<- { '"' ('\"' / [^"])* ('"' / %{StringErr}) }
Keyword	<- { ':' Symbol }

-- Números
Number	<- { [+-]? (Float / Int) }

Float	<- ( FltDot FltExp? / %d+ FltExp) 'f'?
FltDot	<- (%d+ '.' %d+ / %d+ '.' / '.' %d+)
FltExp	<- [eE] [+-]? %d+

Int		<- Hexa / Oct / Bin / Dec
Hexa	<- '0x' ( %x+ / %{HexErr} )
Oct		<- '0o' ( [0-7]+ / %{OctErr} )
Bin		<- '0b' ( [01]+ / %{BinErr} )
Dec		<- %d+

-- Espaços a pular =P
AteEOL	<- [^%nl]*
Comment	<- ';' AteEOL
Sp		<- (%s / Comment)*
]], { print = print })


-- Module
local parser = {}

--- Parse some text
--
-- @param text Text to be parsed
--
-- @return Table with symbols
function parser.parse (text)
	local res, label, suf = g:match (text)
	if res then
		return res
	else
		local lin, col = re.calcline (text, #text - #suf)
		local msg = parseErrors[label]
		suf = suf ~= '' and '"' .. suf:match ('(.-)\n') .. '"' or 'EOL' 
		return nil, table.concat { 'Erro @ ', lin, ',', col, ': ', msg
				, ' em ', suf }
	end
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
