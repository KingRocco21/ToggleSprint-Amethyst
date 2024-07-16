#pragma once
#include "amethyst/runtime/AmethystContext.hpp"
#include "minecraft/src-client/common/client/input/Keymapping.hpp"

class ToggleManager
{
public:
	ToggleManager(AmethystContext* context, int layout1Index, int layout2Index, int keyWhenToggled);
	void toggleKey();
	~ToggleManager();

private:
	Keymapping& mLayout1Keymapping;
	Keymapping& mLayout2Keymapping;
	int mToggledKey;
	bool mToggled;
};