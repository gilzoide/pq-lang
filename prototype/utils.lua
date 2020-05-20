local Utils = {}

function Utils.sublist(t, from, to)
	to = to or #t
	local sub = {}
	for i = from, to do
		table.insert(sub, t[i])
	end
	return sub
end

function Utils.fassert(cond, fmt, ...)
    return assert(cond, string.format(fmt, ...))
end

function Utils.sorted_pairs(t, comp)
    local copy = {}
    for k, v in pairs(t) do table.insert(copy, k) end
    table.sort(copy, comp)
    return coroutine.wrap(function()
        for i, key in ipairs(copy) do
            coroutine.yield(key, t[key])
        end
    end)
end

return Utils

