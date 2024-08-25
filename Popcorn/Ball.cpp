#include "Ball.h"

CBall::CBall()
	: Ball_Pen(0), Ball_Brush(0), Ball_Pos_X(20), Ball_Pos_Y(170), Ball_Speed(3.0), Ball_Direction(M_PI - M_PI_4), Ball_Rect{}, Prev_Ball_Rect{}
{
}

void CBall::Init()
{
	CConfig::Create_Pen_Brush(255, 255, 255, Ball_Pen, Ball_Brush);
}

// Отрисовка шарика
void CBall::Draw(HDC hdc, RECT& paint_area, HPEN bg_pen, HBRUSH bg_brush)
{
	RECT intersection_rect;

	if (!IntersectRect(&intersection_rect, &paint_area, &Ball_Rect))
	{
		return;
	}

	// 1. Очищаем фон
	SelectObject(hdc, bg_pen);
	SelectObject(hdc, bg_brush);

	Ellipse(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right - 1, Prev_Ball_Rect.bottom - 1);

	// 2. Рисуем шарик
	SelectObject(hdc, Ball_Pen);
	SelectObject(hdc, Ball_Brush);

	Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right - 1, Ball_Rect.bottom - 1);
}

void CBall::Move(HWND hwnd, CLevel* level, int platform_pos_x, int platform_width)
{
	int next_pos_x, next_pos_y;

	Prev_Ball_Rect = Ball_Rect;

	next_pos_x = Ball_Pos_X + (int)(Ball_Speed * cos(Ball_Direction));
	next_pos_y = Ball_Pos_Y - (int)(Ball_Speed * sin(Ball_Direction));

	// Корректируем позицию при отражении от рамки
	if (next_pos_x < CConfig::Border_Offset_X)
	{
		next_pos_x = CConfig::Border_Offset_X;
		Ball_Direction = M_PI - Ball_Direction;
	}

	if (next_pos_y < CConfig::Border_Offset_Y)
	{
		next_pos_y = CConfig::Border_Offset_Y;
		Ball_Direction = -Ball_Direction;
	}

	if (next_pos_x > CConfig::Max_Pos_X - CConfig::Ball_Size / 2)
	{
		next_pos_x = CConfig::Max_Pos_X - (next_pos_x - CConfig::Max_Pos_X);
		Ball_Direction = M_PI - Ball_Direction;
	}

	if (next_pos_y > CConfig::Max_Pos_Y)
	{
		next_pos_y = CConfig::Max_Pos_Y - (next_pos_y - CConfig::Max_Pos_Y);
		Ball_Direction = M_PI + (M_PI - Ball_Direction);
	}

	// Корректируем позицию при отражении от платформы
	if (next_pos_y > CConfig::Pos_Y - CConfig::Ball_Size / 2)
	{
		if (next_pos_x >= platform_pos_x && next_pos_x <= platform_pos_x + platform_width)
		{
			next_pos_y = CConfig::Pos_Y - CConfig::Ball_Size;
			Ball_Direction = M_PI + (M_PI - Ball_Direction);
		}
	}

	// Корректируем позицию при отражении от кирпичей
	level->Check_Level_Brick_Hit(next_pos_y, Ball_Direction);

	// Смещаем шарик
	Ball_Pos_X = next_pos_x;
	Ball_Pos_Y = next_pos_y;

	Ball_Rect.left = Ball_Pos_X * CConfig::Global_Scale;
	Ball_Rect.top = Ball_Pos_Y * CConfig::Global_Scale;
	Ball_Rect.right = Ball_Rect.left + CConfig::Ball_Size * CConfig::Global_Scale;
	Ball_Rect.bottom = Ball_Rect.top + CConfig::Ball_Size * CConfig::Global_Scale;

	InvalidateRect(hwnd, &Prev_Ball_Rect, FALSE);
	InvalidateRect(hwnd, &Ball_Rect, FALSE);
}