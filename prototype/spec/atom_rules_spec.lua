local atom_rules = require 'parser.atom_rules'
local lpeg = require 'lpeglabel'

describe("AtomRulePrefix", function()
    it("matches prefix and custom pattern", function()
        local shebang = atom_rules.new_prefix("#!", lpeg.C((1 - lpeg.P('\n'))^0)):pattern()
        local res = shebang:match("#!/usr/bin/env pq")
        assert.are.equal(res, '/usr/bin/env pq')

        local bang_until_space = atom_rules.new_prefix("!"):pattern()
        res = bang_until_space:match("!hi there")
        assert.are.equal("hi", res)
        res = bang_until_space:match("!...---...well,ok")
        assert.are.equal("...---...well,ok", res)

        local just_5 = atom_rules.new_prefix("5", lpeg.P(-1) * lpeg.Cc(true)):pattern()
        res = just_5:match("5")
        assert.are.equal(true, res)
        res = just_5:match("51")
        assert.is_nil(res)

        local no_capture = atom_rules.new_prefix("--", (1 - lpeg.P('\n'))^0):pattern()
        local text = "-- this is a lua comment\n12345"
        res = no_capture:match(text)
        assert.are.equal(#text - 5, res)
    end)
end)

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

    it("matches if prefix and unescaped suffix are matched", function()
        local res
        res = default_pattern:match("''")
        assert.are.equal('', res)
        res = default_pattern:match("'singleword'")
        assert.are.equal('singleword', res)
        res = default_pattern:match("'more words'")
        assert.are.equal('more words', res)
        res = default_pattern:match("'more words and\nnewlines'")
        assert.are.equal('more words and\nnewlines', res)
        res = default_pattern:match("'hi there \\'escaped' ")
        assert.are.equal("hi there 'escaped", res)
    end)

    it("pattern fails if prefix is not met", function()
        local res, err, pos = default_pattern:match("not prefix (')")
        assert.is_nil(res)
        assert.are.equal('fail', err)
        assert.are.equal(1, pos)
    end)

    it("throws label when prefix is met, but suffix is not found", function()
        local text = "'prefix is met, but suffix \\' is not"
        local res, err, pos = default_pattern:match(text)
        assert.is_nil(res)
        assert.are_not.equal('fail', err)
        assert.is_true(pos > 1)
        assert.are.equal(#text + 1, pos)
    end)
end)
