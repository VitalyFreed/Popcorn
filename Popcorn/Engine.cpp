#include "Engine.h"

CEngine::CEngine(): HWnd(0), BG_Pen(0), BG_Brush(0)
{}

// Настройка игры при старте
void CEngine::Init_Engine(HWND hWnd)
{
	HWnd = hWnd;

	CConfig::Create_Pen_Brush(15, 63, 31, BG_Pen, BG_Brush);

	Ball.Init();
	Level.Init();
	Platform.Init();
	Border.Init();

	Platform.Redraw_Platform(HWnd);

	SetTimer(HWnd, Timer_ID, 20, 0);
}

// Отрисовка экрана игры
void CEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{
	Level.Draw(hdc, paint_area);

	Platform.Draw(hdc, BG_Pen, BG_Brush, paint_area);

	/*for (int i = 0; i < 16; i++)
	{
		Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 100, EBrick_Type::Blue, ELetter_Type::O, i);
		Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 130, EBrick_Type::Red, ELetter_Type::O, i);
	}*/

	Ball.Draw(hdc, paint_area, BG_Pen, BG_Brush);

	Border.Draw(hdc, paint_area, BG_Pen, BG_Brush);
}

int CEngine::On_Key_Down(EKey_Type key_type)
{
	switch (key_type)
	{
	case EKey_Type::Left:
		Platform.Pos_X -= Platform.X_Step;

		if (Platform.Pos_X <= CConfig::Border_Offset_X)
		{
			Platform.Pos_X = CConfig::Border_Offset_X;
		}

		Platform.Redraw_Platform(HWnd);
		break;

	case EKey_Type::Right:
		Platform.Pos_X += Platform.X_Step;

		if (Platform.Pos_X >= CConfig::Max_Pos_X - Platform.Width + 1)
		{
			Platform.Pos_X = CConfig::Max_Pos_X - Platform.Width + 1;
		}

		Platform.Redraw_Platform(HWnd);
		break;

	case EKey_Type::Space:
		break;
	}

	return 0;
}

int CEngine::On_Timer()
{
	Ball.Move(HWnd, &Level, Platform.Pos_X, Platform.Width);

	return 0;
}