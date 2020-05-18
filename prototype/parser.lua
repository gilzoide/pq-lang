local Parser = {}
Parser.__index = Parser

function Parser.new()
    return setmetatable({

    }, Parser)
end

function Parser:parse(text)
    local code = {}
    local start = 1
    local last = 1

    for i = 1, #text + 1 do
        local char = string.sub(text, i, i)
        if char == '' or string.match(char, "[ \t]") then
            local token = string.sub(text, start, i - 1)
            if #token > 0 then
                table.insert(code, token)
            end
            start = i + 1
        end
    end

    return code
end

return Parser

