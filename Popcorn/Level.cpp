#include "Level.h"

char Level_01[CConfig::Level_Height][CConfig::Level_Width] =
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

CLevel::CLevel() : Brick_Red_Pen(0), Brick_Blue_Pen(0), Letter_Pen(0), Brick_Red_Brush(0), Brick_Blue_Brush(0), Level_Rect{}
{
}

void CLevel::Init()
{
	Letter_Pen = CreatePen(PS_SOLID, CConfig::Global_Scale, RGB(255, 255, 255));

	CConfig::Create_Pen_Brush(255, 85, 85, Brick_Red_Pen, Brick_Red_Brush);
	CConfig::Create_Pen_Brush(85, 255, 255, Brick_Blue_Pen, Brick_Blue_Brush);

	Level_Rect.left = CConfig::Level_Offset_X * CConfig::Global_Scale;
	Level_Rect.top = CConfig::Level_Offset_X * CConfig::Global_Scale;
	Level_Rect.right = Level_Rect.left + CConfig::Cell_Width * CConfig::Level_Width * CConfig::Global_Scale;
	Level_Rect.bottom = Level_Rect.top + CConfig::Cell_Height * CConfig::Level_Height * CConfig::Global_Scale;
}

// Корректируем позицию при отражении от кирпичей
void CLevel::Check_Level_Brick_Hit(int& next_pos_y, double& ball_direction)
{
	int brick_pos_y = CConfig::Level_Offset_Y + CConfig::Level_Height * CConfig::Cell_Height;

	for (int i = CConfig::Level_Height - 1; i >= 0; i--)
	{
		for (int j = 0; j < CConfig::Level_Width; j++)
		{
			if (!Level_01[i][j])
			{
				continue;
			}

			if (next_pos_y < brick_pos_y)
			{
				next_pos_y = brick_pos_y;
				ball_direction = -ball_direction;
			}
		}

		brick_pos_y -= CConfig::Cell_Height;
	}
}

// Вывод всех кирпичей уровня
void CLevel::Draw(HDC hdc, RECT& paint_area)
{
	RECT intersection_rect;

	if (!IntersectRect(&intersection_rect, &paint_area, &Level_Rect))
	{
		return;
	}

	for (int i = 0; i < CConfig::Level_Height; i++)
	{
		for (int j = 0; j < CConfig::Level_Width; j++)
		{
			Draw_Brick(
				hdc,
				CConfig::Level_Offset_X + j * CConfig::Cell_Width,
				CConfig::Level_Offset_Y + i * CConfig::Cell_Height,
				(EBrick_Type)Level_01[i][j]
			);
		}
	}
}

// Вывод кирпича
void CLevel::Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)
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

	int rect_left = x * CConfig::Global_Scale;
	int rect_top = y * CConfig::Global_Scale;
	int rect_right = (x + CConfig::Brick_Width) * CConfig::Global_Scale;
	int rect_bottom = (y + CConfig::Brick_Height) * CConfig::Global_Scale;
	int round_width = 2 * CConfig::Global_Scale;
	int round_height = 2 * CConfig::Global_Scale;

	RoundRect(hdc, rect_left, rect_top, rect_right, rect_bottom, round_width, round_height);
}

void CLevel::Set_Brick_Letter_Colors(bool is_swicth_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush)
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

// Вывод падающей буквы
void CLevel::Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step)
{
	if (!(brick_type == EBrick_Type::Blue || brick_type == EBrick_Type::Red))
	{
		return;
	}

	bool is_switch_color;
	double offset;
	double rotation_angle; // Преобразование шага в угол поворота
	int brick_half_height = CConfig::Brick_Height * CConfig::Global_Scale / 2;
	int back_part_offset;
	HPEN front_pen, back_pen;
	HBRUSH front_brush, back_brush;
	XFORM xform, old_xform;

	// Корректируем шаг вращения и угол поворота
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
		// Выводим фон
		SelectObject(hdc, back_pen);
		SelectObject(hdc, back_brush);

		Rectangle(
			hdc,
			x,
			y + brick_half_height - CConfig::Global_Scale,
			x + CConfig::Brick_Width * CConfig::Global_Scale,
			y + brick_half_height
		);

		// Выводим передний план
		SelectObject(hdc, front_pen);
		SelectObject(hdc, front_brush);

		Rectangle(
			hdc,
			x,
			y + brick_half_height,
			x + CConfig::Brick_Width * CConfig::Global_Scale,
			y + brick_half_height + CConfig::Global_Scale - 1
		);
	}
	else
	{
		SetGraphicsMode(hdc, GM_ADVANCED);

		// Настраиваем матрицу "переворота" буквы
		xform.eM11 = 1.0f;
		xform.eM12 = 0.0f;
		xform.eM21 = 0.0f;
		xform.eM22 = (float)cos(rotation_angle);
		xform.eDx = (float)x;
		xform.eDy = (float)y + (float)brick_half_height;
		GetWorldTransform(hdc, &old_xform);
		SetWorldTransform(hdc, &xform);

		// Выводим фон
		SelectObject(hdc, back_pen);
		SelectObject(hdc, back_brush);

		offset = 3.0 * (1.0 - fabs(xform.eM22)) * (double)CConfig::Global_Scale;
		back_part_offset = (int)round(offset);
		Rectangle(
			hdc,
			0,
			-brick_half_height - back_part_offset,
			CConfig::Brick_Width * CConfig::Global_Scale,
			brick_half_height - back_part_offset
		);

		// Выводим передний план
		SelectObject(hdc, front_pen);
		SelectObject(hdc, front_brush);

		Rectangle(hdc, 0, -brick_half_height, CConfig::Brick_Width * CConfig::Global_Scale, brick_half_height);

		if (rotation_step > 4 && rotation_step <= 12)
		{
			if (letter_type == ELetter_Type::O)
			{
				SelectObject(hdc, Letter_Pen);
				Ellipse(
					hdc,
					0 + 5 * CConfig::Global_Scale,
					-5 * CConfig::Global_Scale / 2,
					0 + 10 * CConfig::Global_Scale,
					5 * CConfig::Global_Scale / 2
				);
			}
		}

		SetWorldTransform(hdc, &old_xform);
	}
}