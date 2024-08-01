#include "dllmain.hpp"
#include "ToggleManager.hpp"
#include "OptionsFileSearcher.hpp"
#include <amethyst/runtime/events/GameEvents.hpp>
#include <amethyst/runtime/events/InputEvents.hpp>
#include <amethyst/Log.hpp>
#include <minecraft/src-client/common/client/input/Keymapping.hpp>
#include <vector>

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
            Log::Info("[{}] key.sprint found at index {}", MOD_NAME, i);
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
            Log::Info("[{}] key.sprint found at index {}", MOD_NAME, i);
            current.mKeys = {};
            current.mAllowRemap = false;
            break;
        }
    }
    Log::Info("");

    // Register inputs
    event.inputManager.RegisterNewInput("toggle.sprint", findExistingKeys("key.toggle.sprint", { 17 }), true); // 17 = Ctrl

    event.inputManager.RegisterNewInput("walk.forward", findExistingKeys("key.forward", { 87 }), false); // 87 = W
}

void OnStartJoinGame(OnStartJoinGameEvent& event)
{
    g_toggleManager = new ToggleManager{ event.client };

    Amethyst::InputManager& inputs{ *Amethyst::GetContext().mInputManager };
    inputs.AddButtonDownHandler("toggle.sprint", [](FocusImpact focus, IClientInstance& client)
        {
            g_toggleManager->toggle();
        }, false);

    inputs.AddButtonDownHandler("walk.forward", [](FocusImpact focus, IClientInstance& client)
        {
            g_toggleManager->setWalkingForwardState(true);
        }, false);

    inputs.AddButtonUpHandler("walk.forward", [](FocusImpact focus, IClientInstance& client)
        {
            g_toggleManager->setWalkingForwardState(false);
        }, false);
}

void OnTick(UpdateEvent& event)
{
    if (g_toggleManager)
    {
        g_toggleManager->sprint();
    }
}

void OnRequestLeaveGame(OnRequestLeaveGameEvent& event)
{
    delete g_toggleManager;
    g_toggleManager = nullptr;
}

ModFunction void Initialize(AmethystContext& ctx) 
{
    Amethyst::InitializeAmethystMod(ctx);

    Amethyst::GetContext().mFeatures->enableInputSystem = true;
    
    Amethyst::EventBus& events{ Amethyst::GetEventBus() };
    events.AddListener<RegisterInputsEvent>(&RegisterInputs);
    events.AddListener<OnStartJoinGameEvent>(&OnStartJoinGame);
    events.AddListener<UpdateEvent>(&OnTick);
    events.AddListener<OnRequestLeaveGameEvent>(&OnRequestLeaveGame);

    Log::Info("[{}] Mod successfully initialized!", MOD_NAME);
}