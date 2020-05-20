local AtomRules = require 'atom_rules'
local Utils = require 'utils'

local Parser = {}
Parser.__index = Parser

function Parser.new()
    local parser = setmetatable({
        prefixes = {},
        token_rules_between = {},
    }, Parser)

    parser:add_token_rule_between("'", "'", "\\")

    return parser
end

function Parser:add_token_rule_between(prefix, suffix, escape)
    local rule = AtomRules.new_between(prefix, suffix, escape)
    prefix = rule.prefix
    local prefix_first_char = string.sub(rule.prefix, 1, 1)
    self.prefixes[prefix_first_char] = self.prefixes[prefix_first_char] or {}
    Utils.fassert(
        self.prefixes[prefix_first_char][prefix] == nil,
        "Prefix %q is already registered",
        prefix
    )
    self.prefixes[prefix_first_char][prefix] = rule
    table.insert(self.token_rules_between, rule)
end

local function sort_prefix(a, b)
    return #a.key > #b.key
end

function Parser:parse(text)
    local stack = { {} }
    local start = 1
    local last = 1

    local i = 1
    while i <= #text do
        local suffix = string.sub(text, i)
        local whitespace = string.match(suffix, "^%s+")
        if whitespace then
            -- TODO treat newlines
            i = i + #whitespace
            goto continue
        end

        local first_char = string.sub(suffix, 1, 1)
        local token, advance
        if self.prefixes[first_char] then
            for prefix, rule in Utils.sorted_pairs(self.prefixes[first_char], sort_prefix) do
                if string.sub(suffix, 1, #prefix) == prefix then
                    token, advance = rule:read(suffix)
                    break
                end
            end
        end
        
        if token == nil then
            token, advance = string.match(suffix, "(%S+)()")
        end

        if #token > 0 then
            table.insert(stack[#stack], token)
        end
        i = i + advance

        ::continue::
    end
    if #stack == 1 then stack = stack[1] end
    return stack
end

return Parser

