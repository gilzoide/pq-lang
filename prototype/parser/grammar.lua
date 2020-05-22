local lpeg = require 'lpeglabel'

local Grammar = {}
Grammar.__index = Grammar

--[[
-- Chunk <- Space {| (Line Space)* |} -> flatten_single_table !.
-- Line <- {| (Expr SpaceButEOL)+ |} -> flatten_single_table
-- Expr <- Block / SExpr / Atom
--
-- Block <- '{' Space (Line Space)* '}'^ErrClosingBlock
-- SExpr <- '(' Space {| (Expr Space)* |} ')'^ErrClosingParentheses
-- Atom <- {: PrefixedAtom / GenericAtom :}
--
-- Space <- [ \t\r\n]*
-- SpaceButEOL <- [ \t\r]*
-- GenericAtom <- [^ \t\r\n(){}]+
--
-- PrefixedAtom -- user defined, by default just fails
--]]

local function flatten_single_table(t)
    if #t == 1 and type(t[1]) == 'table' then
        return t[1]
    else
        return t
    end
end

local Space = lpeg.S(" \t\r\n")^0
local SpaceButEOL = lpeg.S(" \t\r")^0
local GenericAtom = (1 - lpeg.S(" \t\r\n(){}"))^1

local Chunk = Space * lpeg.Ct((lpeg.V("Line") * Space)^0) / flatten_single_table * (-1)
local Line = lpeg.Ct((lpeg.V("Expr") * SpaceButEOL)^1) / flatten_single_table
local Expr = lpeg.V("Block") + lpeg.V("SExpr") + lpeg.V("Atom")

local Block =
    lpeg.P("{")
    * Space
    * (lpeg.V("Line") * Space)^0
    * (lpeg.P("}") + lpeg.T('ErrClosingBlock'))
local SExpr =
    lpeg.P("(")
    * Space
    * lpeg.Ct((lpeg.V("Expr") * Space)^0)
    * (lpeg.P(")") + lpeg.T('ErrClosingParentheses'))
local Atom = lpeg.Cs(lpeg.V("PrefixedAtom") + GenericAtom)

local PrefixedAtom_default = lpeg.P(false)

function Grammar.create_base() 
    return setmetatable({
        rules = {
            "Chunk",
            Chunk = Chunk,
            Line = Line,
            Expr = Expr,
            Block = Block,
            SExpr = SExpr,
            Atom = Atom,
            PrefixedAtom = PrefixedAtom_default,
        }
    }, Grammar)
end

function Grammar:compile()
    return lpeg.P(self.rules)
end

function Grammar:set_prefixed_atom_rules(rules)
    local prefixed_atom = PrefixedAtom_default
    for i, rule in ipairs(rules) do
        prefixed_atom = prefixed_atom + rule:pattern()
    end
    self.rules.PrefixedAtom = prefixed_atom
end

return Grammar

