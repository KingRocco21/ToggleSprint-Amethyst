#include "dllmain.hpp"
#include "ToggleManager.hpp"
#include "OptionsFileSearcher.hpp"
#include <amethyst/runtime/events/GameEvents.hpp>
#include <amethyst/runtime/events/InputEvents.hpp>
#include <amethyst/Log.hpp>

static ToggleManager* g_toggleManager;

void RegisterInputs(RegisterInputsEvent& event)
{
    event.inputManager.RegisterNewInput("toggle.sprint", findExistingKeys("key.toggle.sprint", { 17 }), true); // 17 = Ctrl

    event.inputManager.RegisterNewInput("conditional.sprint", findExistingKeys("key.forward", { 87 }), false); // 87 = W

    // Use this to search through the registered inputs for names or indexes.
    /*
    std::vector<Keymapping> keymappings1{ context->mOptions->mKeyboardRemappings.at(0)->mKeymappings };
    for (int i{ 0 }; i != keymappings1.size(); ++i)
    {
        Keymapping current{ keymappings1.at(i) };
        Log::Info("{}: {}", i, current.mAction);
    }
    Log::Info("");
    std::vector<Keymapping> keymappings2{ context->mOptions->mKeyboardRemappings.at(1)->mKeymappings };
    for (int i{ 0 }; i != keymappings2.size(); ++i)
    {
        Keymapping current{ keymappings2.at(i) };
        Log::Info("{}: {}", i, current.mAction);
    }
    */
}

void OnStartJoinGame(OnStartJoinGameEvent& event)
{
    g_toggleManager = new ToggleManager{ event.client };

    Amethyst::InputManager& inputs{ *Amethyst::GetContext().mInputManager };
    inputs.AddButtonDownHandler("toggle.sprint", [](FocusImpact focus, IClientInstance& client)
        {
            g_toggleManager->toggle();
        }, false);

    inputs.AddButtonDownHandler("conditional.sprint", [](FocusImpact focus, IClientInstance& client)
        {
            g_toggleManager->sprintIfToggled();
        }, false);
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
    events.AddListener<OnStartJoinGameEvent>(&OnStartJoinGame);
    events.AddListener<RegisterInputsEvent>(&RegisterInputs);
    events.AddListener<OnRequestLeaveGameEvent>(&OnRequestLeaveGame);

    Log::Info("[ToggleSprint] Mod successfully initialized!");
}