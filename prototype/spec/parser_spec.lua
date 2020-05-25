local Parser = require 'parser'

describe("When parsing input,", function()
    local parser = Parser.new()

    it("returns list of atoms in line", function()
        local text = "1 2 3   +4'\tfive"
        local res = parser:parse(text)
        assert.are.same({'1', '2', '3', '+4\'', 'five'}, res)
    end)

    it("returns one list per non-empty line", function()
        local text = [[

        one
        per line

        skips empty lines
        ]]
        local res = parser:parse(text)
        assert.are.same({
            {'one'},
            {'per', 'line'},
            {'skips', 'empty', 'lines'},
        }, res)
    end)

    it("unescaped single quotes delimit a single atom", function()
        local text = "first 'second and still second' third"
        local res = parser:parse(text)
        assert.are.same({'first', 'second and still second', 'third'}, res)

        text = [[first 'second\'escaped' third]]
        res = parser:parse(text)
        assert.are.same({'first', "second'escaped", 'third'}, res)
    end)

    it("single quotes delimiter must be closed", function()
        local text = [[this 'is an error\']]
        assert.has.errors(function() parser:parse(text) end)
    end)

    it("parenthesized atoms become sublist", function()
        local text = "outside (inside)  (another inside one)"
        local res = parser:parse(text)
        assert.are.same({'outside', {'inside'}, {'another', 'inside', 'one'}}, res)
    end)

    it("whole parenthesized line counts as a single list", function()
        local text = [[

        (first line)
        

        (second line) with outside atoms
        third line
        ]]
        local res = parser:parse(text)
        assert.are.same({
            {'first', 'line'},
            {{'second', 'line'}, 'with', 'outside', 'atoms'},
            {'third', 'line'},
        }, res)
    end)

    it("parenthesis across multiple lines become a single list", function()
        local text = [[
        (this is 
            a single
            list
        )
        (
            likewise
        )
        back to normal
        ]]
        local res = parser:parse(text)
        assert.are.same({
            {'this', 'is', 'a', 'single', 'list'},
            {'likewise'},
            {'back', 'to', 'normal'},
        }, res)
    end)

    describe("errors with unbalanced parenthesis", function()
        local lines = {
            "(",
            ")",
            "(()",
            "())",
        }

        for i, text in ipairs(lines) do
            it(string.format("-- %q", line), function()
                assert.has.errors(function() parser:parse(text) end)
            end)
        end
    end)

    it("groups blocks lines", function()
        local text = [[
        block {
            first line
            second line
            (third
                line)
        }

        after
        ]]
        local res = parser:parse(text)
        assert.are.same({
            {
                'block',
                {
                    {'first', 'line'},
                    {'second', 'line'},
                    {'third', 'line'},
                },
            },
            {'after'},
        }, res)
    end)
end)
