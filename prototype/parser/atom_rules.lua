local Utils = require 'utils'
local lpeg = require 'lpeglabel'

--- Module handler for rule creation
local AtomRules = {}

--- Prefix atom rule

--- Suffix atom rule

--- Infix atom rule

--- Between atom rule
local AtomRuleBetween = {}
AtomRuleBetween.__index = AtomRuleBetween

AtomRuleBetween.kind = 'between'

function AtomRuleBetween.new(prefix, suffix, escape)
    assert(type(prefix) == 'string' and #prefix > 0, "Token rule prefix must be a non-empty string")
    suffix = suffix or prefix
    assert(type(suffix) == 'string' and #suffix > 0, "Token rule suffix must be a non-empty string")
    escape = escape or '\\'
    assert(type(escape) == 'string' and #escape > 0, "Token rule escape must be a non-empty string")

    return setmetatable({
        prefix = prefix,
        suffix = suffix,
        escape = escape,
    }, AtomRuleBetween)
end

function AtomRuleBetween:pattern()
    local prefix = lpeg.P(self.prefix) / ''
    local escaped_suffix = lpeg.P(self.escape .. self.suffix) / self.suffix
    local between = (escaped_suffix + (1 - lpeg.P(self.suffix)))^0
    local suffix = lpeg.P(self.suffix) / ''
    local label_unmatched_suffix = lpeg.T(AtomRules.label_unmatched_suffix .. self.prefix)
    return lpeg.Cs(prefix * between * (suffix + label_unmatched_suffix)) / 1
end

--- AtomRules definitions
AtomRules.label_unmatched_suffix = 'ErrUnmatchedSuffix_'

function AtomRules.new_between(prefix, suffix, escape)
    return AtomRuleBetween.new(prefix, suffix, escape)
end

return AtomRules

