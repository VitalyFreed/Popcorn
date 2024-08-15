#include "Engine.h"

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

HWND HWnd;
HPEN Brick_Red_Pen, Brick_Blue_Pen, Platform_Circle_Pen, Platform_Inner_Pen, Highlight_Pen, Letter_Pen, BG_Pen;
HBRUSH Brick_Red_Brush, Brick_Blue_Brush, Platform_Circle_Brush, Platform_Inner_Brush, BG_Brush;

const int Global_Scale = 3;
const int Brick_Width = 15;
const int Brick_Height = 7;
const int Cell_Width = Brick_Width + 1;
const int Cell_Height = Brick_Height + 1;
const int Level_Offset_X = 8;
const int Level_Offset_Y = 6;
const int Level_Width = 14; // ������ ������ � �������
const int Level_Height = 12; // ������ ������ � �������
const int Circle_Size = 7;
const int Platform_Pos_Y = 140;
const int Platform_Height = 7;

int Inner_Platform_Width = 21;
int Platform_Pos_X = 0;
int Platform_X_Step = Global_Scale * 2;
int Platform_Width = 28;

RECT Platform_Rect, Prev_Platform_Rect;
RECT Level_Rect;

char Level_01[Level_Width][Level_Height] =
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

void Redraw_Platform()
{
    Prev_Platform_Rect = Platform_Rect;

    Platform_Rect.left = (Level_Offset_X + Platform_Pos_X) * Global_Scale;
    Platform_Rect.top = Platform_Pos_Y * Global_Scale;
    Platform_Rect.right = Platform_Rect.left + Platform_Width * Global_Scale;
    Platform_Rect.bottom = Platform_Rect.top + Platform_Height * Global_Scale;

    InvalidateRect(HWnd, &Prev_Platform_Rect, FALSE);
    InvalidateRect(HWnd, &Platform_Rect, FALSE);
}

// ��������� ���� ��� ������
void Init_Engine(HWND hWnd)
{
    HWnd = hWnd;

    Highlight_Pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
    Letter_Pen = CreatePen(PS_SOLID, Global_Scale, RGB(255, 255, 255));

    Create_Pen_Brush(15, 63, 31, BG_Pen, BG_Brush);
    Create_Pen_Brush(255, 85, 85, Brick_Red_Pen, Brick_Red_Brush);
    Create_Pen_Brush(85, 255, 255, Brick_Blue_Pen, Brick_Blue_Brush);
    Create_Pen_Brush(151, 0, 0, Platform_Circle_Pen, Platform_Circle_Brush);
    Create_Pen_Brush(0, 128, 192, Platform_Inner_Pen, Platform_Inner_Brush);

    Level_Rect.left = Level_Offset_X * Global_Scale;
    Level_Rect.top = Level_Offset_X * Global_Scale;
    Level_Rect.right = Level_Rect.left + Cell_Width * Level_Width * Global_Scale;
    Level_Rect.bottom = Level_Rect.top + Cell_Height * Level_Height * Global_Scale;

    Redraw_Platform();
}

// ����� �������
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

void Set_Brick_Letter_Colors(bool is_swicth_color, HPEN &front_pen, HBRUSH &front_brush, HPEN &back_pen, HBRUSH &back_brush)
{
    if (is_swicth_color)
    {
        front_pen = Brick_Red_Pen;
        front_brush = Brick_Red_Brush;

        back_pen = Brick_Blue_Pen;
        back_brush = Brick_Blue_Brush;
    }
    else
    {
        front_pen = Brick_Blue_Pen;
        front_brush = Brick_Blue_Brush;

        back_pen = Brick_Red_Pen;
        back_brush = Brick_Red_Brush;
    }
}

// ����� �������� �����
void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step)
{
    if (!(brick_type == EBrick_Type::Blue || brick_type == EBrick_Type::Red))
    {
        return;
    }

    bool is_switch_color;
    double offset;
    double rotation_angle; // �������������� ���� � ���� ��������
    int brick_half_height = Brick_Height * Global_Scale / 2;
    int back_part_offset;
    HPEN front_pen, back_pen;
    HBRUSH front_brush, back_brush;
    XFORM xform, old_xform;

    // ������������ ��� �������� � ���� ��������
    rotation_step = rotation_step % 16;

    if (rotation_step < 8)
    {
        rotation_angle = 2.0 * M_PI / 16.0 * (double)rotation_step;
    }
    else
    {
        rotation_angle = 2.0 * M_PI / 16.0 * (double)(8 - rotation_step);
    }

    if (rotation_step > 4 && rotation_step <= 12)
    {
        is_switch_color = brick_type == EBrick_Type::Blue;
        Set_Brick_Letter_Colors(is_switch_color, front_pen, front_brush, back_pen, back_brush);
    }
    else
    {
        is_switch_color = brick_type == EBrick_Type::Red;
        Set_Brick_Letter_Colors(is_switch_color, front_pen, front_brush, back_pen, back_brush);
    }

    if (rotation_step == 4 || rotation_step == 12)
    {
        // ������� ���
        SelectObject(hdc, back_pen);
        SelectObject(hdc, back_brush);

        Rectangle(hdc, x, y + brick_half_height - Global_Scale, x + Brick_Width * Global_Scale, y + brick_half_height);

        // ������� �������� ����
        SelectObject(hdc, front_pen);
        SelectObject(hdc, front_brush);

        Rectangle(hdc, x, y + brick_half_height, x + Brick_Width * Global_Scale, y + brick_half_height + Global_Scale - 1);
    }
    else
    {
        SetGraphicsMode(hdc, GM_ADVANCED);

        // ����������� ������� "����������" �����
        xform.eM11 = 1.0f;
        xform.eM12 = 0.0f;
        xform.eM21 = 0.0f;
        xform.eM22 = (float)cos(rotation_angle);
        xform.eDx = (float)x;
        xform.eDy = (float)y + (float)brick_half_height;
        GetWorldTransform(hdc, &old_xform);
        SetWorldTransform(hdc, &xform);

        // ������� ���
        SelectObject(hdc, back_pen);
        SelectObject(hdc, back_brush);

        offset = 3.0 * (1.0 - fabs(xform.eM22)) * (double)Global_Scale;
        back_part_offset = (int)round(offset);
        Rectangle(hdc, 0, -brick_half_height - back_part_offset, Brick_Width * Global_Scale, brick_half_height - back_part_offset);

        // ������� �������� ����
        SelectObject(hdc, front_pen);
        SelectObject(hdc, front_brush);

        Rectangle(hdc, 0, -brick_half_height, Brick_Width * Global_Scale, brick_half_height);

        if (rotation_step > 4 && rotation_step <= 12)
        {
            if (letter_type == ELetter_Type::O)
            {
                SelectObject(hdc, Letter_Pen);
                Ellipse(hdc, 0 + 5 * Global_Scale, -5 * Global_Scale / 2, 0 + 10 * Global_Scale, 5 * Global_Scale / 2);
            }
        }

        SetWorldTransform(hdc, &old_xform);
    }
}

// ����� ���� �������� ������
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

// ��������� ���������
void Draw_Platform(HDC hdc, int x, int y)
{
    SelectObject(hdc, BG_Pen);
    SelectObject(hdc, BG_Brush);

    Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);

    // 1. ������ ������� ������ ���������
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

    // 2. ������ ���� �� ����� ������
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

    // 3. ������ ��������� ���������
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

// ��������� ������ ����
void Draw_Frame(HDC hdc, RECT &paint_area)
{
    RECT intersection_rect;

    if (IntersectRect(&intersection_rect, &paint_area, &Level_Rect))
    {
        Draw_Level(hdc);
    }

    if (IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))
    {
        Draw_Platform(hdc, Level_Offset_X + Platform_Pos_X, Platform_Pos_Y);
    }
    
    /*for (int i = 0; i < 16; i++)
    {
        Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 100, EBrick_Type::Blue, ELetter_Type::O, i);
        Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 130, EBrick_Type::Red, ELetter_Type::O, i);
    }*/
}

int On_Key_Down(EKey_Type key_type)
{
    switch(key_type)
    {
    case EKey_Type::Left:
        Platform_Pos_X -= Platform_X_Step;
        Redraw_Platform();
        break;

    case EKey_Type::Right:
        Platform_Pos_X += Platform_X_Step;
        Redraw_Platform();
        break;

    case EKey_Type::Space:
        break;
    }

    return 0;
}