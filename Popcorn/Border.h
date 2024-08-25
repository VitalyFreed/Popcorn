#pragma once

#include "Config.h"

class CBorder
{
public:
    CBorder();

    void Init();
    void Draw(HDC hdc, RECT& paint_area, HPEN bg_pen, HBRUSH bg_brush);

private:
    HPEN Border_Blue_Pen, Border_White_Pen;
    HBRUSH Border_Blue_Brush, Border_White_Brush;

    void Draw_Element(HDC hdc, int x, int y, bool is_top_border, HPEN bg_pen, HBRUSH bg_brush);
};