local Utils = require 'utils'

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
    }, AtomRuleBetween)
end

function AtomRuleBetween:read(text)
    local escape_length = #self.escape
    local search_start = #self.prefix + 1
    local suffix_start, suffix_end

    local components = {}
    while true do
        suffix_start, suffix_end = string.find(text, self.suffix, search_start)
        Utils.fassert(
            suffix_start,
            "Expected closing %q for atom starting with %q",
            self.suffix, self.prefix
        )
        local component = string.sub(text, search_start, suffix_start - 1)
        if string.sub(component, -escape_length) == self.escape then
            table.insert(components, string.sub(component, 1, -escape_length - 1))
            search_start = suffix_end + 1
        else
            table.insert(components, component)
            break
        end
    end

    return table.concat(components, self.suffix), suffix_end
end

--- Module handler for rule creation
local AtomRules = {}

function AtomRules.new_between(prefix, suffix, escape)
    return AtomRuleBetween.new(prefix, suffix, escape)
end

return AtomRules

