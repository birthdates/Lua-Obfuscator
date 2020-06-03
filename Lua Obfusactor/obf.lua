function obf()
	local thing = [[
	]]

	thing = thing .. codeTable
	return thing:gsub(".", function(bb) return "\\" .. bb:byte() end) or thing .. "\"";
end
