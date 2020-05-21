local AtomRules = require 'parser.atom_rules'
local Grammar = require 'parser.grammar'
local Utils = require 'utils'

local re = require 'relabel'

local Parser = {}
Parser.__index = Parser

function Parser.new()
    local base_grammar = Grammar.create_base()
    local parser = setmetatable({
        grammar = base_grammar,
        compiled_grammar = nil,
        dirty = true,
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
    if self.dirty then
        self.compiled_grammar = self.grammar:compile()
    end

    local res, err, pos = self.compiled_grammar:match(text)
    if not res then
        local line, col = re.calcline(text, pos)
        local msg = "Error at line " .. line .. " (col " .. col .. "): "
        error(msg .. err)
    end
    return res
end

return Parser

