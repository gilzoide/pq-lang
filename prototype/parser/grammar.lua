local lpeg = require 'lpeglabel'

local Grammar = {}
Grammar.__index = Grammar

--[[
-- Chunk <- Space {| (Line Space)* |} !.
-- Line <- {| (Expr SpaceButEOL)+ |}
-- Expr <- Block / SExpr / Atom
--
-- Block <- '{' Space (Line Space)* '}'^ErrClosingBlock
-- SExpr <- '(' Space (Expr Space)* ')'^ErrClosingParentheses
-- Atom <- { PrefixedAtom / GenericAtom }
--
-- Space <- [ \t\r\n]*
-- SpaceButEOL <- [ \t\r]*
-- GenericAtom <- [^ \t\r\n(){}]+
--
-- PrefixedAtom -- user defined, by default just fails
--]]

local Space = lpeg.S(" \t\r\n")^0
local SpaceButEOL = lpeg.S(" \t\r")^0
local GenericAtom = (1 - lpeg.S(" \t\r\n(){}"))^1

local Chunk = Space * lpeg.Ct((lpeg.V("Line") * Space)^0) * (-1)
local Line = lpeg.Ct((lpeg.V("Expr") * SpaceButEOL)^1)
local Expr = lpeg.V("Block") + lpeg.V("SExpr") + lpeg.V("Atom")

local Block =
    lpeg.P("{")
    * Space
    * (lpeg.V("Line") * Space)^0
    * (lpeg.P("}") + lpeg.T('ErrClosingBlock'))
local SExpr =
    lpeg.P("(")
    * Space
    * (lpeg.V("Expr") * Space)^0
    * (lpeg.P(")") + lpeg.T('ErrClosingParentheses'))
local Atom = lpeg.C(lpeg.V("PrefixedAtom") + GenericAtom)

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

return Grammar

