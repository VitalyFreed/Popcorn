#pragma once

#include "Border.h"
#include "Ball.h"
#include "Platform.h"

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
    HWND HWnd;
    HPEN BG_Pen;
    HBRUSH BG_Brush;

    CEngine();

    void Init_Engine(HWND hWnd);
    void Draw_Frame(HDC hdc, RECT &paint_area);
    int On_Key_Down(EKey_Type key_type);
    int On_Timer();

private:
    CBall Ball;
    CLevel Level;
    CPlatform Platform;
    CBorder Border;
};
