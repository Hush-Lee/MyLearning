#pragma once
#include <SDL2/SDL_render.h>
#include "Npc.hpp"
class DialogManager {
public:
    void startDialog(NPC* npc);
    void update();
    void render(SDL_Renderer* renderer);
    void nextLine();
    bool inDialog() const;

private:
    NPC* currentNPC = nullptr;
    bool active = false;
};

