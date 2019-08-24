local utils = require 'utils'
local parser = require 'parser'

local Context = {}
Context.__index = Context

function Context:parse(text)
	return parser:match(text)
end

function Context:get(key)
	assert(type(key) == 'string', "Only strings may be keys for scopes")
	local value
	for i = #self.scopes, 1, -1 do
		value = self.scopes[i][key]
		if value ~= nil then break end
	end
	return value
end

function Context:set(key, value)
	assert(type(key) == 'string', "Only strings may be keys for scopes")
	self.scopes[#self.scopes][key] = value
end

function Context:push_scope()
	table.insert(self.scopes, {})
end

function Context:eval(code)
	local t = type(code)
	if t == 'nil' then return nil
	elseif t == 'string' then return self:get(code)
	elseif t == 'table' then
		if #code == 0 then return nil end
		local f = self:eval(code[1])
		local args = {}
		for i = 2, #code do
			table.insert(args, self:eval(code[i]))
		end
		return f(table.unpack(args))
	else return code
	end
end

return function()
	return setmetatable({
		scopes = { {} },
		env = {},
	}, Context)
end
