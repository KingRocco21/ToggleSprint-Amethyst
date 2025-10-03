#pragma once
#include <mc/src-client/common/client/game/ClientInstance.hpp>

class ToggleManager
{
public:
	ToggleManager(ClientInstance& client);
	void toggle();
	void setWalkingForwardState(bool isWalkingForward) { mIsWalkingForward = isWalkingForward; };
	void trySprint();

private:
	ClientInstance& mClient;
	bool mIsToggled;
	bool mIsWalkingForward;
};