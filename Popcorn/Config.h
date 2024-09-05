#pragma once

#include <Windows.h>

#define _USE_MATH_DEFINES
#include <math.h>

class CConfig
{
public:
    static const int Global_Scale = 3;
    static const int Border_Offset_X = 6;
    static const int Border_Offset_Y = 4;
    static const int Level_Width = 12; // Ширина уровня в ячейках
    static const int Level_Height = 14; // Высота уровня в ячейках
    static const int Level_Offset_X = 8;
    static const int Level_Offset_Y = 6;
    static const int Brick_Width = 15;
    static const int Brick_Height = 7;
    static const int Cell_Width = 16;
    static const int Cell_Height = 8;
    static const int Ball_Size = 4;
    static const int Max_Pos_X = Level_Offset_X + Level_Width * Cell_Width;
    static const int Max_Pos_Y = 199 - Ball_Size;
    static const int Platform_Pos_Y = 185;
    static const int FPS = 20;

    static void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush);
};