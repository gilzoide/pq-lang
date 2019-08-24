local Utils = {}

function Utils.sublist(t, from, to)
	to = to or #t
	local sub = {}
	for i = from, to do
		table.insert(sub, t[i])
	end
	return sub
end

return Utils

