#include "Active_Brick.h"

CActive_Brick::CActive_Brick() : Fade_Step(0), Brick_Rect{ 0 }
{
}

void CActive_Brick::Draw(HDC hdc, RECT& paint_area)
{
	HBRUSH brush;
	HPEN pen;

	CConfig::Create_Pen_Brush(
		85 - Fade_Step * 85 / Max_Fade_Step,
		255 - Fade_Step * 255 / Max_Fade_Step,
		255 - Fade_Step * 255 / Max_Fade_Step,
		pen,
		brush
	);

	SelectObject(hdc, pen);
	SelectObject(hdc, brush);

	Brick_Rect.left = (CConfig::Level_Offset_X + CConfig::Cell_Width) * CConfig::Global_Scale;
	Brick_Rect.top = (CConfig::Level_Offset_Y + CConfig::Cell_Height) * CConfig::Global_Scale;
	Brick_Rect.right = Brick_Rect.left + CConfig::Brick_Width * CConfig::Global_Scale;
	Brick_Rect.bottom = Brick_Rect.top + CConfig::Brick_Height * CConfig::Global_Scale;

	RoundRect(
		hdc,
		Brick_Rect.left,
		Brick_Rect.top,
		Brick_Rect.right,
		Brick_Rect.bottom,
		2 * CConfig::Global_Scale,
		2 * CConfig::Global_Scale
	);
}

void CActive_Brick::Act(HWND hwnd)
{
	if (Fade_Step < Max_Fade_Step)
	{
		++Fade_Step;

		InvalidateRect(hwnd, &Brick_Rect, FALSE);
	}
}