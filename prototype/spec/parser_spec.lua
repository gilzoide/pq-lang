local Parser = require 'parser'

describe("When parsing line,", function()
    local parser = Parser.new()

    it("returns list of tokens", function()
        local line = "1 2 3   +4'\tfive"
        local res = parser:parse(line)
        assert.are.same({'1', '2', '3', '+4\'', 'five'}, res)
    end)

    describe("errors with unbalanced parenthesis", function()
        local lines = {
            "(",
            ")",
            "(()",
            "())",
        }

        for i, line in ipairs(lines) do
            it(string.format("-- %q", line), function()
                assert.has.errors(function() parser:parse(line) end)
            end)
        end
    end)
end)
