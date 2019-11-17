#pragma once

#include <gameserver.hpp>
#include <unordered_map>
#include <unordered_set>

struct loginserver_data
{
	std::unordered_set<std::shared_ptr<gameserver>> gses_;
	std::unordered_map<std::string, std::shared_ptr<gameserver>> user_to_gameserver_;
	std::unordered_map<std::string, std::string> validated_username_to_cookie_;
};