#include "dllmain.hpp"
#include "ToggleManager.hpp"
#include "OptionsFileSearcher.hpp"
#include <amethyst/runtime/mod/Mod.hpp>
#include <amethyst/runtime/ModContext.hpp>
#include <amethyst/runtime/events/GameEvents.hpp>
#include <amethyst/runtime/events/InputEvents.hpp>
#include <amethyst/Log.hpp>
#include <mc/src-client/common/client/input/Keymapping.hpp>
#include <vector>

// Testing

static ToggleManager* g_toggleManager;

void RegisterInputs(RegisterInputsEvent& event)
{
    // Hide Sprint in the settings so it can't conflict with "key.toggle.sprint".
    AmethystContext& context{ Amethyst::GetContext() };

    std::vector<Keymapping>& keymappings1{ context.mOptions->mKeyboardRemappings.at(0)->mKeymappings };
    for (int i{ 0 }; i != keymappings1.size(); ++i)
    {
        Keymapping& current{ keymappings1.at(i) };
        //Log::Info("{}: {}", i, current.mAction);
        if (current.mAction == "key.sprint")
        {
            Log::Info("key.sprint found at index {}. Disabling.", i);
            current.mKeys = {};
            current.mAllowRemap = false;
            break;
        }
    }

    std::vector<Keymapping>& keymappings2{ context.mOptions->mKeyboardRemappings.at(1)->mKeymappings };
    for (int i{ 0 }; i != keymappings2.size(); ++i)
    {
        Keymapping& current{ keymappings2.at(i) };
        //Log::Info("{}: {}", i, current.mAction);
        if (current.mAction == "key.sprint")
        {
            Log::Info("key.sprint found at index {}. Disabling.", i);
            current.mKeys = {};
            current.mAllowRemap = false;
            break;
        }
    }

    // Register inputs
    // https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
    // https://www.rapidtables.com/convert/number/hex-to-decimal.html?x=31
    std::vector<int> toggleSprintKeys{ OptionsFileSearcher::findExistingKeys("key.toggle.sprint", { 17 }) }; // 17 = Ctrl
    Amethyst::InputAction& toggleSprint{ context.mInputManager->RegisterNewInput("toggle.sprint", toggleSprintKeys, true, Amethyst::KeybindContext::Gameplay) };
    toggleSprint.addButtonDownHandler([](FocusImpact focus, ClientInstance& client)
        {
            if (g_toggleManager) g_toggleManager->toggle();
            else Log::Info("Toggle Sprint pressed outside of gameplay. Ignoring button press.");
            return Amethyst::InputPassthrough::ModOnly;
        });

    Amethyst::InputAction& walkForwards{ context.mInputManager->GetVanillaInput("up") };
    walkForwards.addButtonDownHandler([](FocusImpact focus, ClientInstance& client)
        {
            if (g_toggleManager) g_toggleManager->setWalkingForwardState(true);
            else Log::Info("Walk Forwards pressed outside of gameplay. Ignoring button press.");
            return Amethyst::InputPassthrough::Passthrough;
        });
    walkForwards.addButtonUpHandler([](FocusImpact focus, ClientInstance& client)
        {
            if (g_toggleManager) g_toggleManager->setWalkingForwardState(false);
            else Log::Info("Walk Forwards pressed outside of gameplay. Ignoring button press.");
            return Amethyst::InputPassthrough::Passthrough;
        });

    // Testing
    std::vector<int> hotbar1Keys{ OptionsFileSearcher::findExistingKeys("key.hotbar.1", { 49 }) }; // 49 = 1 key
    Amethyst::InputAction& hotbar1{ context.mInputManager->RegisterNewInput("hotkey.1", hotbar1Keys, true, Amethyst::KeybindContext::Screen) };
    hotbar1.addButtonDownHandler([](FocusImpact focus, ClientInstance& client)
        {
            std::cout << "Hotbar 1 pressed\n";
            return Amethyst::InputPassthrough::ModOnly;
        });
}

void OnStartJoinGame(OnStartJoinGameEvent& event)
{
    g_toggleManager = new ToggleManager{ event.client };
}

void OnUpdate(UpdateEvent& event)
{
    if (g_toggleManager) g_toggleManager->trySprint();
}

void OnRequestLeaveGame(OnRequestLeaveGameEvent& event)
{
    delete g_toggleManager;
    g_toggleManager = nullptr;
}

ModFunction void Initialize(AmethystContext& ctx, const Amethyst::Mod& mod) 
{
    Amethyst::InitializeAmethystMod(ctx, mod);

    Amethyst::EventBus& events{ Amethyst::GetEventBus() };
    events.AddListener<RegisterInputsEvent>(&RegisterInputs);
    events.AddListener<OnStartJoinGameEvent>(&OnStartJoinGame);
    events.AddListener<UpdateEvent>(&OnUpdate);
    events.AddListener<OnRequestLeaveGameEvent>(&OnRequestLeaveGame);
}
