#include "ToggleManager.hpp"
#include "amethyst/Log.hpp"
#include "amethyst/runtime/AmethystContext.hpp"
#include "minecraft/src-client/common/client/options/Options.hpp"
#include <vector>

ToggleManager::ToggleManager(AmethystContext* context, int layout1Index, int layout2Index, int keyWhenToggled)
	: mLayout1Keymapping{ context->mOptions->mKeyboardRemappings.at(0)->mKeymappings.at(layout1Index) }
	, mLayout2Keymapping{ context->mOptions->mKeyboardRemappings.at(1)->mKeymappings.at(layout2Index) }
	, mToggledKey{ keyWhenToggled }
	, mToggled{ false }
{
	toggleKey();
}

void ToggleManager::toggleKey()
{
	if (mToggled)
	{
		auto begin{ mLayout1Keymapping.mKeys.begin() };
		auto end{ mLayout1Keymapping.mKeys.end() };
		for (auto p{ begin }; p != end; ++p)
		{
			if (*p == mToggledKey)
			{
				p = mLayout1Keymapping.mKeys.erase(p);
			}
		}

		begin = mLayout2Keymapping.mKeys.begin();
		end = mLayout2Keymapping.mKeys.end();
		for (auto p{ begin }; p != end; ++p)
		{
			if (*p == mToggledKey)
			{
				p = mLayout2Keymapping.mKeys.erase(p);
			}
		}
		Log::Info("{} untoggled.", mLayout1Keymapping.mAction);
	}
	else {
		mLayout1Keymapping.mKeys.push_back(mToggledKey);
		mLayout2Keymapping.mKeys.push_back(mToggledKey);
		Log::Info("{} toggled.", mLayout1Keymapping.mAction);
	}

	Log::Info("Keys are now:");
	for (int key : mLayout1Keymapping.mKeys)
	{
		if (key == 17)
		{
			Log::Info("Ctrl");
		}
		else
		{
			Log::Info("{}", static_cast<char>(key));
		}
	}

	mToggled = !mToggled;
}

ToggleManager::~ToggleManager()
{
	if (mToggled)
		toggleKey();
}