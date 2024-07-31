#include "ToggleManager.hpp"
#include <amethyst/Log.hpp>
#include <minecraft/src-client/common/client/player/LocalPlayer.hpp>

ToggleManager::ToggleManager(ClientInstance& client)
	: mClient{ client }
	, mIsToggled{ true } // Sprint is toggled on by default.
{}

void ToggleManager::toggle()
{
	if (!mIsToggled)
	{
		mIsToggled = true;
		Log::Info("[ToggleSprint] Sprint toggled.");
	}
	else
	{
		mClient.getLocalPlayer()->setSprinting(false);
		mIsToggled = false;
		Log::Info("[ToggleSprint] Sprint untoggled.");
	}
}

void ToggleManager::sprintIfToggled()
{
	if (mIsToggled)
	{
		mClient.getLocalPlayer()->setSprinting(true);
	}
}