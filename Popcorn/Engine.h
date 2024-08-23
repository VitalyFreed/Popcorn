#pragma once

#include <Windows.h>

#define _USE_MATH_DEFINES
#include <math.h>

enum class ELetter_Type
{
    None,
    O,
};

enum class EBrick_Type
{
    None,
    Red,
    Blue,
};

enum class EKey_Type
{
    Left,
    Right,
    Space,
};

const int Timer_ID = WM_USER + 1;

class CEngine
{
public:
    static const int Level_Width = 12; // Ширина уровня в ячейках
    static const int Level_Height = 14; // Высота уровня в ячейках

    CEngine();

    void Init_Engine(HWND hWnd);
    void Draw_Frame(HDC hdc, RECT& paint_area);
    int On_Key_Down(EKey_Type key_type);
    int On_Timer();

private:
    HWND HWnd;
    HPEN Brick_Red_Pen, Brick_Blue_Pen, Platform_Circle_Pen, Platform_Inner_Pen, Highlight_Pen, Letter_Pen, BG_Pen, Ball_Pen, Border_Blue_Pen, Border_White_Pen;
    HBRUSH Brick_Red_Brush, Brick_Blue_Brush, Platform_Circle_Brush, Platform_Inner_Brush, BG_Brush, Ball_Brush, Border_Blue_Brush, Border_White_Brush;

    int Inner_Platform_Width;
    int Platform_Pos_X;
    int Platform_X_Step;
    int Platform_Width;

    int Ball_Pos_X, Ball_Pos_Y;
    double Ball_Speed, Ball_Direction;

    RECT Platform_Rect, Prev_Platform_Rect;
    RECT Level_Rect;
    RECT Ball_Rect, Prev_Ball_Rect;

    static const int Global_Scale = 3;
    static const int Brick_Width = 15;
    static const int Brick_Height = 7;
    static const int Cell_Width = Brick_Width + 1;
    static const int Cell_Height = Brick_Height + 1;
    static const int Level_Offset_X = 8;
    static const int Level_Offset_Y = 6;
    static const int Circle_Size = 7;
    static const int Platform_Pos_Y = 185;
    static const int Platform_Height = 7;
    static const int Ball_Size = 4;
    static const int Max_Pos_X = Level_Offset_X + Level_Width * Cell_Width;
    static const int Max_Pos_Y = 199 - Ball_Size;
    static const int Border_Offset_X = 6;
    static const int Border_Offset_Y = 4;

    void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush);
    void Redraw_Platform();
    void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type);
    void Set_Brick_Letter_Colors(bool is_swicth_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush);
    void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step);
    void Draw_Level(HDC hdc);
    void Draw_Platform(HDC hdc, int x, int y);
    void Draw_Ball(HDC hdc);
    void Draw_Border(HDC hdc, int x, int y, bool is_top_border);
    void Draw_Bounds(HDC hdc, RECT& paint_area);
    void Check_Level_Brick_Hit(int& next_pos_y);
    void Move_Ball();
};
