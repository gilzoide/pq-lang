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
        atom_rules_prefix = {},
        atom_rules_between = {},
        expr_rules_block = {},
    }, Parser)

    parser:add_atom_rule_between("'", "'", "\\")
    parser:add_expr_rule_block('{', '}')

    return parser
end

function Parser:add_atom_rule_prefix(prefix, atom_pattern)
    local rule = AtomRules.new_prefix(prefix, atom_pattern)
    table.insert(self.atom_rules_prefix, rule)
end

function Parser:add_atom_rule_between(prefix, suffix, escape)
    local rule = AtomRules.new_between(prefix, suffix, escape)
    table.insert(self.atom_rules_between, rule)
end

function Parser:add_expr_rule_block(prefix, suffix)
    local rule = AtomRules.new_block(prefix, suffix)
    table.insert(self.expr_rules_block, rule)
end

local function sort_prefix(a, b)
    return #a.key > #b.key
end

function Parser:parse(text)
    if self.dirty then
        self.grammar:set_prefixed_atom_rules(
            self.expr_rules_block,
            self.atom_rules_prefix,
            self.atom_rules_between
        )
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

