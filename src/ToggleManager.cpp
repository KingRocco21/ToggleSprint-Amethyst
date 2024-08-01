#include "ToggleManager.hpp"
#include <amethyst/runtime/ModContext.hpp>
#include <amethyst/Log.hpp>
#include <minecraft/src-client/common/client/player/LocalPlayer.hpp>

ToggleManager::ToggleManager(ClientInstance& client)
	: mClient{ client }
	, mIsToggled{ true } // Sprint is toggled on by default.
	, mIsWalkingForward{ false }
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

void ToggleManager::sprint()
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