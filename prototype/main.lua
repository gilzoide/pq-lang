local context = require('context')()

context:set('a', 1)
context:set('b', 2)
context:set('+', function(a, b) 
	return a + b
end)

local code = context:parse('+ a b')
print(context:eval(code))

