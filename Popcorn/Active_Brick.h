#pragma once

#include "Config.h"

class CActive_Brick
{
public:
    CActive_Brick();

    void Draw(HDC hdc, RECT& paint_area);
    void Act(HWND hwnd);

private:
    int Fade_Step;
    static const int Max_Fade_Step = 80;

    RECT Brick_Rect;
};
