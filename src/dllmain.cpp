#include "dllmain.hpp"
#include "ToggleManager.hpp"
#include "OptionsFileSearcher.hpp"
#include "amethyst/Log.hpp"

AmethystContext* context;
ToggleManager* toggleManager;

void RegisterInputs(Amethyst::InputManager* input)
{
    const std::string fullActionName{ "key.toggle.sprint" };
    input->RegisterNewInput("toggle.sprint", findExistingKeys(fullActionName, { 17 } ), true);

    /* Use this to find your desired action index.
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

void OnStartJoinGame(ClientInstance* client)
{
    // "key.sprint" is at index 17 for layout 1, and 34 for layout 2. 87 = W.
    toggleManager = new ToggleManager{ context, 17, 34, { 87 } };

    context->mInputManager.AddButtonDownHandler("toggle.sprint", [](FocusImpact focus, ClientInstance& client)
        {
            toggleManager->toggleKey();
        }, false);
}

void OnRequestLeaveGame()
{
    delete toggleManager;
    toggleManager = nullptr;
}

ModFunction void Initialize(AmethystContext* ctx) 
{
    context = ctx;

    ctx->mEventManager.registerInputs.AddListener(RegisterInputs);
    ctx->mEventManager.onStartJoinGame.AddListener(OnStartJoinGame);
    ctx->mEventManager.onRequestLeaveGame.AddListener(OnRequestLeaveGame);

    Log::Info("[ToggleSprint] Mod successfully initialized!");
}