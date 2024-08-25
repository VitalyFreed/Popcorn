#pragma once

#include "Config.h"
#include "Level.h"

class CEngine;
class CLevel;
class CPlatform;

class CBall
{
public:
    double Ball_Direction;

    HPEN Ball_Pen;
    HBRUSH Ball_Brush;

    CBall();
    void Init();

    void Draw(HDC hdc, RECT& paint_area, HPEN bg_pen, HBRUSH bg_brush);
    void Move(HWND hwnd, CLevel* level, int platform_pos_x, int platform_width);

private:
    int Ball_Pos_X, Ball_Pos_Y;
    double Ball_Speed;

    RECT Ball_Rect, Prev_Ball_Rect;
};
