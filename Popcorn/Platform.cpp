#include "Platform.h"

CPlatform::CPlatform()
	: Inner_Width(21), Pos_X(CConfig::Border_Offset_X), X_Step(CConfig::Global_Scale * 2), Width(28),
	Platform_Circle_Pen(0), Platform_Inner_Pen(0), Highlight_Pen(0), Platform_Circle_Brush(0), Platform_Inner_Brush(0),
	Platform_Rect{}, Prev_Platform_Rect{}
{}

void CPlatform::Init()
{
	Highlight_Pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));

	CConfig::Create_Pen_Brush(151, 0, 0, Platform_Circle_Pen, Platform_Circle_Brush);
	CConfig::Create_Pen_Brush(0, 128, 192, Platform_Inner_Pen, Platform_Inner_Brush);
}

void CPlatform::Redraw_Platform(HWND hwnd)
{
	Prev_Platform_Rect = Platform_Rect;

	Platform_Rect.left = Pos_X * CConfig::Global_Scale;
	Platform_Rect.top = CConfig::Platform_Pos_Y * CConfig::Global_Scale;
	Platform_Rect.right = Platform_Rect.left + Width * CConfig::Global_Scale;
	Platform_Rect.bottom = Platform_Rect.top + Height * CConfig::Global_Scale;

	InvalidateRect(hwnd, &Prev_Platform_Rect, FALSE);
	InvalidateRect(hwnd, &Platform_Rect, FALSE);
}

// Отрисовка платформы
void CPlatform::Draw(HDC hdc, HPEN bg_pen, HBRUSH bg_brush, RECT& paint_area)
{
	RECT intersection_rect;

	if (!IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))
	{
		return;
	}

	SelectObject(hdc, bg_pen);
	SelectObject(hdc, bg_brush);

	Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);

	// 1. Рисуем боковые шарики платформы
	SelectObject(hdc, Platform_Circle_Pen);
	SelectObject(hdc, Platform_Circle_Brush);

	int x = Pos_X, y = CConfig::Platform_Pos_Y;

	Ellipse(
		hdc,
		x * CConfig::Global_Scale,
		y * CConfig::Global_Scale,
		(x + Circle_Size) * CConfig::Global_Scale,
		(y + Circle_Size) * CConfig::Global_Scale
	);
	Ellipse(
		hdc,
		(x + Inner_Width) * CConfig::Global_Scale,
		y * CConfig::Global_Scale,
		(x + Circle_Size + Inner_Width) * CConfig::Global_Scale,
		(y + Circle_Size) * CConfig::Global_Scale
	);

	// 2. Рисуем блик на левом шарике
	SelectObject(hdc, Highlight_Pen);

	Arc(
		hdc,
		(x + 1) * CConfig::Global_Scale,
		(y + 1) * CConfig::Global_Scale,
		(x + Circle_Size - 1) * CConfig::Global_Scale,
		(y + Circle_Size - 1) * CConfig::Global_Scale,
		(x + 1 + 1) * CConfig::Global_Scale,
		(y + 1) * CConfig::Global_Scale,
		(x + 1) * CConfig::Global_Scale,
		(y + 1 + 2) * CConfig::Global_Scale
	);

	// 3. Рисуем основание платформы
	SelectObject(hdc, Platform_Inner_Pen);
	SelectObject(hdc, Platform_Inner_Brush);

	RoundRect(
		hdc,
		(x + 4) * CConfig::Global_Scale,
		(y + 1) * CConfig::Global_Scale,
		(x + 4 + Inner_Width - 1) * CConfig::Global_Scale,
		(y + 1 + 5) * CConfig::Global_Scale, 3 * CConfig::Global_Scale,
		3 * CConfig::Global_Scale
	);
}