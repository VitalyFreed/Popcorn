#pragma once

#include "Config.h"

class CPlatform
{
public:
    int Pos_X;
    int Width;
    int X_Step;

    CPlatform();

    void Init();
    void Redraw_Platform(HWND hwnd);
    void Draw(HDC hdc, HPEN bg_pen, HBRUSH bg_brush, RECT& paint_area);

private:
    static const int Height = 7;
    static const int Circle_Size = 7;

    int Inner_Width;

    HPEN Platform_Circle_Pen, Platform_Inner_Pen, Highlight_Pen;
    HBRUSH Platform_Circle_Brush, Platform_Inner_Brush;
    RECT Platform_Rect, Prev_Platform_Rect;
};
