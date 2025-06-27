#pragma once
#pragma once
#include<SDL2/SDL_render.h>
#include<string>
#include<vector>
class NPC {
public:
    NPC(int x, int y, const std::string& dialogFile);
    void render(SDL_Renderer* renderer);
    bool isNear(int playerX, int playerY);
    const std::string& getCurrentLine();
    void nextLine();
    bool hasDialogLeft() const;

private:
    SDL_Rect rect;
    std::vector<std::string> dialogLines;
    size_t currentLine;
};

