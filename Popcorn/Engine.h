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

class CEngine;
class CLevel;
class CPlatform;

class CBall
{
public:
    static const int Ball_Size = 4;

    double Ball_Direction;

    HPEN Ball_Pen;
    HBRUSH Ball_Brush;

    CBall();
    void Init();

    void Draw(HDC hdc, RECT &paint_area, CEngine* engine);
    void Move(CEngine *engine, CLevel *level, CPlatform *platform);

private:
    int Ball_Pos_X, Ball_Pos_Y;
    double Ball_Speed;

    RECT Ball_Rect, Prev_Ball_Rect;
};

class CLevel
{
public:
    static const int Level_Width = 12; // Ширина уровня в ячейках
    static const int Level_Height = 14; // Высота уровня в ячейках
    static const int Level_Offset_X = 8;
    static const int Level_Offset_Y = 6;
    static const int Brick_Width = 15;
    static const int Brick_Height = 7;
    static const int Cell_Width = Brick_Width + 1;
    static const int Cell_Height = Brick_Height + 1;

    void Init();
    void Check_Level_Brick_Hit(int& next_pos_y, double& ball_direction);
    void Draw(HDC hdc, RECT& paint_area);

private:
    HPEN Brick_Red_Pen, Brick_Blue_Pen, Letter_Pen;
    HBRUSH Brick_Red_Brush, Brick_Blue_Brush;
    RECT Level_Rect;

    void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type);
    void Set_Brick_Letter_Colors(bool is_swicth_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush);
    void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step);
};

class CPlatform
{
public:
    static const int Pos_Y = 185;

    int Pos_X;
    int Width;
    int X_Step;

    CPlatform();

    void Init();
    void Redraw_Platform(CEngine* engine);
    void Draw(HDC hdc, CEngine* engine, RECT& paint_area);

private:
    static const int Height = 7;
    static const int Circle_Size = 7;

    int Inner_Width;

    HPEN Platform_Circle_Pen, Platform_Inner_Pen, Highlight_Pen;
    HBRUSH Platform_Circle_Brush, Platform_Inner_Brush;
    RECT Platform_Rect, Prev_Platform_Rect;
};

class CBorder
{
public:
    static const int Border_Offset_X = 6;
    static const int Border_Offset_Y = 4;

    void Init();
    void Draw(HDC hdc, RECT& paint_area, CEngine* engine);

private:
    HPEN Border_Blue_Pen, Border_White_Pen;
    HBRUSH Border_Blue_Brush, Border_White_Brush;

    void Draw_Element(HDC hdc, int x, int y, bool is_top_border, CEngine* engine);
};

class CEngine
{
public:
    static const int Global_Scale = 3;
    static const int Max_Pos_X = CLevel::Level_Offset_X + CLevel::Level_Width * CLevel::Cell_Width;
    static const int Max_Pos_Y = 199 - CBall::Ball_Size;

    HWND HWnd;
    HPEN BG_Pen;
    HBRUSH BG_Brush;

    CEngine();

    void Init_Engine(HWND hWnd);
    void Draw_Frame(HDC hdc, RECT &paint_area);
    int On_Key_Down(EKey_Type key_type);
    int On_Timer();
    static void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush);

private:
    CBall Ball;
    CLevel Level;
    CPlatform Platform;
    CBorder Border;
};
