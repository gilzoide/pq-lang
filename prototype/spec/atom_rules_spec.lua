local atom_rules = require 'parser.atom_rules'
local lpeg = require 'lpeglabel'

describe("AtomRuleBetween", function()
    local default_rule = atom_rules.new_between("'")
    local default_pattern = default_rule:pattern()

    it("uses prefix as suffix, if ommited", function()
        local rule = atom_rules.new_between('"')
        assert.are.same('"', rule.suffix)
    end)

    it("expects prefix string and optional string suffix and escape", function()
        local function create_between(prefix, suffix, escape)
            return function() return atom_rules.new_between(prefix, suffix, escape) end
        end
        assert.has_no.errors(create_between('prefix', 'suffix', 'escape'))
        assert.has.errors(create_between(0, 'suffix', 'escape'))
        assert.has.errors(create_between('prefix', 1, 'escape'))
        assert.has.errors(create_between('prefix', 'suffix', 2))
        assert.has.errors(create_between(nil))
    end)

    it("parses correctly", function()
        local res
        res = default_pattern:match("''")
        assert.are.same('', res)
        res = default_pattern:match("'singleword'")
        assert.are.same('singleword', res)
        res = default_pattern:match("'more words'")
        assert.are.same('more words', res)
        res = default_pattern:match("'more words and\nnewlines'")
        assert.are.same('more words and\nnewlines', res)
        res = default_pattern:match("'hi there \\'escaped' ")
        assert.are.same("hi there 'escaped", res)
    end)

    it("pattern fails if prefix is not met", function()
        local res, err, pos = default_pattern:match("not prefix (')")
        assert.is_nil(res)
        assert.are.same('fail', err)
        assert.are.equal(1, pos)
    end)

    it("throws label when prefix is met, but suffix is not found", function()
        local text = "'prefix is met, but suffix \\' is not"
        local res, err, pos = default_pattern:match(text)
        assert.is_nil(res)
        assert.is_not.equal('fail', err)
        assert.is_true(pos > 1)
        assert.are.equal(#text + 1, pos)
    end)
end)
