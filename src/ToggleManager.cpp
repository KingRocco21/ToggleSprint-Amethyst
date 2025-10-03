#include "ToggleManager.hpp"
#include <amethyst/runtime/ModContext.hpp>
#include <amethyst/Log.hpp>
#include <mc/src-client/common/client/player/LocalPlayer.hpp>

ToggleManager::ToggleManager(ClientInstance& client)
	: mClient{ client }
	, mIsToggled{ true } // Sprint is toggled on by default.
	, mIsWalkingForward{ false }
{
}

void ToggleManager::toggle()
{
	if (!mIsToggled)
	{
		mIsToggled = true;
		Log::Info("Sprint toggled.");
	}
	else
	{
		mClient.getLocalPlayer()->setSprinting(false);
		mIsToggled = false;
		Log::Info("Sprint untoggled.");
	}
}

void ToggleManager::trySprint()
{
	if (mIsToggled && mIsWalkingForward)
	{
		LocalPlayer* player{ mClient.getLocalPlayer() };
		if (player)
		{
			player->setSprinting(true);
		}
	}
}