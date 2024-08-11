#include "Engine.h"

enum EBrick_Type
{
    None,
    Red,
    Blue,
};

HPEN Brick_Red_Pen, Brick_Blue_Pen, Platform_Circle_Pen, Platform_Inner_Pen, Highlight_Pen;
HBRUSH Brick_Red_Brush, Brick_Blue_Brush, Platform_Circle_Brush, Platform_Inner_Brush;

const int Global_Scale = 3;
const int Brick_Width = 15;
const int Brick_Height = 7;
const int Cell_Width = Brick_Width + 1;
const int Cell_Height = Brick_Height + 1;
const int Level_Offset_X = 8;
const int Level_Offset_Y = 6;
const int Circle_Size = 7;
int Inner_Platform_Width = 21;

char Level_01[14][12] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &brush)
{
    pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
    brush = CreateSolidBrush(RGB(r, g, b));
}

// Настройка игры при старте
void Init()
{
    Highlight_Pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));

    Create_Pen_Brush(255, 85, 85, Brick_Red_Pen, Brick_Red_Brush);
    Create_Pen_Brush(85, 255, 255, Brick_Blue_Pen, Brick_Blue_Brush);
    Create_Pen_Brush(151, 0, 0, Platform_Circle_Pen, Platform_Circle_Brush);
    Create_Pen_Brush(0, 128, 192, Platform_Inner_Pen, Platform_Inner_Brush);
}

// Вывод кирпича
void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)
{
    HPEN pen;
    HBRUSH brush;

    switch (brick_type)
    {
    case EBrick_Type::None:
        return;
    case EBrick_Type::Red:
        pen = Brick_Red_Pen;
        brush = Brick_Red_Brush;
        break;
    case EBrick_Type::Blue:
        pen = Brick_Blue_Pen;
        brush = Brick_Blue_Brush;
        break;
    default:
        return;
    }

    SelectObject(hdc, pen);
    SelectObject(hdc, brush);

    int rect_left = x * Global_Scale;
    int rect_top = y * Global_Scale;
    int rect_right = (x + Brick_Width) * Global_Scale;
    int rect_bottom = (y + Brick_Height) * Global_Scale;
    int round_width = 2 * Global_Scale;
    int round_height = 2 * Global_Scale;

    RoundRect(hdc, rect_left, rect_top, rect_right, rect_bottom, round_width, round_height);
}

// Вывод всех кирпичей уровня
void Draw_Level(HDC hdc)
{
    for (int i = 0; i < 14; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            Draw_Brick(hdc, Level_Offset_X + j * Cell_Width, Level_Offset_Y + i * Cell_Height, (EBrick_Type)Level_01[i][j]);
        }
    }
}

// Отрисовка платформы
void Draw_Platform(HDC hdc, int x, int y)
{
    // 1. Рисуем боковые шарики платформы
    SelectObject(hdc, Platform_Circle_Pen);
    SelectObject(hdc, Platform_Circle_Brush);

    Ellipse(hdc,
        x * Global_Scale,
        y * Global_Scale,
        (x + Circle_Size) * Global_Scale,
        (y + Circle_Size) * Global_Scale
    );
    Ellipse(hdc,
        (x + Inner_Platform_Width) * Global_Scale,
        y * Global_Scale,
        (x + Circle_Size + Inner_Platform_Width) * Global_Scale,
        (y + Circle_Size) * Global_Scale
    );

    // 2. Рисуем блик на левом шарике
    SelectObject(hdc, Highlight_Pen);

    Arc(
        hdc,
        (x + 1) * Global_Scale,
        (y + 1) * Global_Scale,
        (x + Circle_Size - 1) * Global_Scale,
        (y + Circle_Size - 1) * Global_Scale,
        (x + 1 + 1) * Global_Scale,
        (y + 1) * Global_Scale,
        (x + 1) * Global_Scale,
        (y + 1 + 2) * Global_Scale
    );

    // 3. Рисуем основание платформы
    SelectObject(hdc, Platform_Inner_Pen);
    SelectObject(hdc, Platform_Inner_Brush);

    RoundRect(
        hdc,
        (x + 4) * Global_Scale,
        (y + 1) * Global_Scale,
        (x + 4 + Inner_Platform_Width - 1) * Global_Scale,
        (y + 1 + 5) * Global_Scale, 3 * Global_Scale,
        3 * Global_Scale
    );
}

// Отрисовка экрана игры
void Draw_Frame(HDC hdc)
{
    Draw_Level(hdc);
    Draw_Platform(hdc, 50, 100);
}