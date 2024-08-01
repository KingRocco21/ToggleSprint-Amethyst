#pragma once
#include <minecraft/src-client/common/client/game/ClientInstance.hpp>

class ToggleManager
{
public:
	ToggleManager(ClientInstance& client);
	void toggle();
	void setWalkingForwardState(bool isWalkingForward) { mIsWalkingForward = isWalkingForward; };
	void sprint();

private:
	ClientInstance& mClient;
	bool mIsToggled;
	bool mIsWalkingForward;
};