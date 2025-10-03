#pragma once
#include <vector>
#include <string_view>

namespace OptionsFileSearcher
{
	std::vector<int> findExistingKeys(std::string_view actionName, const std::vector<int>& keysIfNotFound);
}