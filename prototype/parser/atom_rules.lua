local Utils = require 'utils'
local lpeg = require 'lpeglabel'

--- Prefix atom rule

--- Suffix atom rule

--- Infix atom rule

--- Between atom rule
local AtomRuleBetween = {
    kind = 'between'
}
AtomRuleBetween.__index = AtomRuleBetween

function AtomRuleBetween.new(prefix, suffix, escape)
    assert(type(prefix) == 'string' and #prefix > 0, "Token rule prefix must be a non-empty string")
    suffix = suffix or prefix
    assert(type(suffix) == 'string' and #suffix > 0, "Token rule suffix must be a non-empty string")
    escape = escape or '/'
    assert(type(escape) == 'string' and #escape > 0, "Token rule escape must be a non-empty string")

    return setmetatable({
        prefix = prefix,
        suffix = suffix,
        escape = escape,
        pattern = lpeg.Cs(lpeg.P(prefix) * (lpeg.P(escape .. suffix) / suffix + (1 - lpeg.P(suffix)))^0 * lpeg.P(suffix))
    }, AtomRuleBetween)
end

--- Module handler for rule creation
local AtomRules = {}

function AtomRules.new_between(prefix, suffix, escape)
    return AtomRuleBetween.new(prefix, suffix, escape)
end

return AtomRules

