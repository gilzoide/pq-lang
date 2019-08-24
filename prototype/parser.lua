local lpeg = require 'lpeglabel'
local re = require 'relabel'

local parser = re.compile([=[
Axiom <- Sp (!. / Line)

Line  <- {| (Expr SpLine)+ |}
Expr <- Block / SExpr / Atom

Block <- "{" Sp (Line Sp)* "}"
SExpr <- "(" Sp (Expr Sp)* ")"

Atom <- { Float / Int / String / Symbol }
Symbol <- [^(){};%s]+

Digits <- %d+
Sign <- [+-]?
Int <- (Sign Digits) -> tonumber
Float <- (Sign (FloatDot FloatExp? / %d+ FloatExp)) -> tonumber
FloatDot <- %d* "." Digits
FloatExp <- [eE] Sign Digits

String <- '"' (!'"' Character)* '"'
Character <- "\" [abfnrtv'"]
           / "\" [0-2] [0-7] [0-7]
           / "\" [0-7] [0-7]?
           / .

Comment <- ";" [^%nl]*
Space <- %s / Comment
Sp <- Space*
SpLine <- (!%nl Space)*
]=], {
	tonumber = tonumber,
})

return parser

