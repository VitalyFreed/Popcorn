#include "Border.h"

CBorder::CBorder(): Border_Blue_Pen(0), Border_White_Pen(0), Border_Blue_Brush(0), Border_White_Brush(0)
{
}

void CBorder::Init()
{
	CConfig::Create_Pen_Brush(85, 255, 255, Border_Blue_Pen, Border_Blue_Brush);
	CConfig::Create_Pen_Brush(255, 255, 255, Border_White_Pen, Border_White_Brush);
}

// Отрисовка рамки уровня
void CBorder::Draw(HDC hdc, RECT& paint_area, HPEN bg_pen, HBRUSH bg_brush)
{
	// Левая рамка
	for (int i = 0; i < 50; i++)
	{
		Draw_Element(hdc, 2, 1 + i * 4, false, bg_pen, bg_brush);
	}

	// Правая рамка
	for (int i = 0; i < 50; i++)
	{
		Draw_Element(hdc, 201, 1 + i * 4, false, bg_pen, bg_brush);
	}

	// Верхняя рамка
	for (int i = 0; i < 50; i++)
	{
		Draw_Element(hdc, 3 + i * 4, 0, true, bg_pen, bg_brush);
	}
}

// Отрисовка элемент рамки уровня
void CBorder::Draw_Element(HDC hdc, int x, int y, bool is_top_border, HPEN bg_pen, HBRUSH bg_brush)
{
	// Основная линия
	SelectObject(hdc, Border_Blue_Pen);
	SelectObject(hdc, Border_Blue_Brush);

	if (is_top_border)
	{
		Rectangle(hdc,
			x * CConfig::Global_Scale,
			(y + 1) * CConfig::Global_Scale,
			(x + 4) * CConfig::Global_Scale,
			(y + 4) * CConfig::Global_Scale
		);
	}
	else
	{
		Rectangle(hdc,
			(x + 1) * CConfig::Global_Scale,
			y * CConfig::Global_Scale,
			(x + 4) * CConfig::Global_Scale,
			(y + 4) * CConfig::Global_Scale
		);
	}

	// Белая кайма
	SelectObject(hdc, Border_White_Pen);
	SelectObject(hdc, Border_White_Brush);

	if (is_top_border)
	{
		Rectangle(hdc,
			x * CConfig::Global_Scale,
			y * CConfig::Global_Scale,
			(x + 4) * CConfig::Global_Scale,
			(y + 1) * CConfig::Global_Scale
		);
	}
	else
	{
		Rectangle(hdc,
			x * CConfig::Global_Scale,
			y * CConfig::Global_Scale,
			(x + 1) * CConfig::Global_Scale,
			(y + 4) * CConfig::Global_Scale
		);
	}

	// Перфорация
	SelectObject(hdc, bg_pen);
	SelectObject(hdc, bg_brush);

	if (is_top_border)
	{
		Rectangle(
			hdc,
			(x + 2) * CConfig::Global_Scale,
			(y + 2) * CConfig::Global_Scale,
			(x + 3) * CConfig::Global_Scale,
			(y + 3) * CConfig::Global_Scale
		);
	}
	else
	{
		Rectangle(hdc,
			(x + 2) * CConfig::Global_Scale,
			(y + 1) * CConfig::Global_Scale,
			(x + 3) * CConfig::Global_Scale,
			(y + 2) * CConfig::Global_Scale
		);
	}
}