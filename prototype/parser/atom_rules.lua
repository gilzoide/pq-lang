local Utils = require 'utils'
local lpeg = require 'lpeglabel'

local locale = lpeg.locale()

--- Module handler for rule creation
local AtomRules = {}

--- Prefix atom rule
local AtomRulePrefix = {}
AtomRulePrefix.__index = AtomRulePrefix

AtomRulePrefix.kind = 'prefix'

function AtomRulePrefix.new(prefix, atom_pattern)
    assert(type(prefix) == 'string' and #prefix > 0, "Atom rule prefix must be a non-empty string")
    if atom_pattern == nil then atom_pattern = lpeg.C((1 - locale.space)^1) end

    return setmetatable({
        prefix = prefix,
        atom_pattern = atom_pattern,
    }, AtomRulePrefix)
end

function AtomRulePrefix:pattern()
    return lpeg.P(self.prefix) * lpeg.P(self.atom_pattern)
end

--- Suffix atom rule

--- Infix atom rule

--- Block expression rule
local ExprRuleBlock = {}
ExprRuleBlock.__index = ExprRuleBlock

function ExprRuleBlock.new(prefix, suffix)
    assert(type(prefix) == 'string' and #prefix > 0, "Block rule prefix must be a non-empty string")
    suffix = suffix or prefix

    return setmetatable({
        prefix = prefix,
        suffix = suffix,
    }, ExprRuleBlock)
end

function ExprRuleBlock:pattern()
    local space = locale.space^0
    local prefix = lpeg.P(self.prefix)
    local between = (lpeg.V("Line") * space)^0
    local suffix = lpeg.P(self.suffix)
    local label_unmatched_suffix = lpeg.T(AtomRules.label_unmatched_suffix .. self.prefix)
    return prefix * space * between * (suffix + label_unmatched_suffix)
end

--- Between atom rule
local AtomRuleBetween = {}
AtomRuleBetween.__index = AtomRuleBetween

AtomRuleBetween.kind = 'between'

function AtomRuleBetween.new(prefix, suffix, escape)
    assert(type(prefix) == 'string' and #prefix > 0, "Atom rule prefix must be a non-empty string")
    suffix = suffix or prefix
    assert(
        escape == nil or (type(escape) == 'string' and #escape > 0), 
        "Atom rule escape must be nil or a non-empty string"
    )

    return setmetatable({
        prefix = prefix,
        suffix = suffix,
        escape = escape,
    }, AtomRuleBetween)
end

function AtomRuleBetween:pattern()
    local prefix = lpeg.P(self.prefix) / ''
    local escaped_suffix = self.escape and lpeg.P(self.escape .. self.suffix) / self.suffix or lpeg.P(false)
    local between = (escaped_suffix + (1 - lpeg.P(self.suffix)))^0
    local suffix = lpeg.P(self.suffix) / ''
    local label_unmatched_suffix = lpeg.T(AtomRules.label_unmatched_suffix .. self.prefix)
    return lpeg.Cs(prefix * between * (suffix + label_unmatched_suffix)) / 1
end

--- AtomRules definitions
AtomRules.label_unmatched_suffix = 'ErrUnmatchedSuffix_'

AtomRules.new_prefix = AtomRulePrefix.new
AtomRules.new_between = AtomRuleBetween.new
AtomRules.new_block = ExprRuleBlock.new

return AtomRules

