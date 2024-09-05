#pragma once

#include "Config.h"
#include "Active_Brick.h"

enum class EBrick_Type
{
    None,
    Red,
    Blue,
};

enum class ELetter_Type
{
    None,
    O,
};

class CLevel
{
public:
    CActive_Brick Active_Brick;

    CLevel();

    void Init();
    void Check_Level_Brick_Hit(int& next_pos_y, double& ball_direction);
    void Draw(HWND hwnd, HDC hdc, RECT& paint_area);

private:
    static char Level_01[CConfig::Level_Height][CConfig::Level_Width];

    HPEN Brick_Red_Pen, Brick_Blue_Pen, Letter_Pen;
    HBRUSH Brick_Red_Brush, Brick_Blue_Brush;
    RECT Level_Rect;

    void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type);
    void Set_Brick_Letter_Colors(bool is_swicth_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush);
    void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step);
};