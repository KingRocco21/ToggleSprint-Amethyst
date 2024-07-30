#pragma once
#include <minecraft/src-client/common/client/game/ClientInstance.hpp>

class ToggleManager
{
public:
	ToggleManager(ClientInstance& client);
	void toggle();
	void sprintIfToggled();

private:
	ClientInstance& mClient;
	bool mIsToggled;
};