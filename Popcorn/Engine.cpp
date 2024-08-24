#include "Engine.h"

char Level_01[CLevel::Level_Height][CLevel::Level_Width] =
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

CBall::CBall()
	: Ball_Pos_X(20), Ball_Pos_Y(170), Ball_Speed(3.0), Ball_Direction(M_PI - M_PI_4)
{
}

void CBall::Init()
{
	CEngine::Create_Pen_Brush(255, 255, 255, Ball_Pen, Ball_Brush);
}

// Отрисовка шарика
void CBall::Draw(HDC hdc, RECT& paint_area, CEngine *engine)
{
	RECT intersection_rect;

	if (!IntersectRect(&intersection_rect, &paint_area, &Ball_Rect))
	{
		return;
	}

	// 1. Очищаем фон
	SelectObject(hdc, engine->BG_Pen);
	SelectObject(hdc, engine->BG_Brush);

	Ellipse(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right - 1, Prev_Ball_Rect.bottom - 1);

	// 2. Рисуем шарик
	SelectObject(hdc, Ball_Pen);
	SelectObject(hdc, Ball_Brush);

	Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right - 1, Ball_Rect.bottom - 1);
}

void CBall::Move(CEngine *engine, CLevel *level, CPlatform *platform)
{
	int next_pos_x, next_pos_y;

	Prev_Ball_Rect = Ball_Rect;

	next_pos_x = Ball_Pos_X + (int)(Ball_Speed * cos(Ball_Direction));
	next_pos_y = Ball_Pos_Y - (int)(Ball_Speed * sin(Ball_Direction));

	// Корректируем позицию при отражении от рамки
	if (next_pos_x < CBorder::Border_Offset_X)
	{
		next_pos_x = CBorder::Border_Offset_X;
		Ball_Direction = M_PI - Ball_Direction;
	}

	if (next_pos_y < CBorder::Border_Offset_Y)
	{
		next_pos_y = CBorder::Border_Offset_Y;
		Ball_Direction = -Ball_Direction;
	}

	if (next_pos_x > CEngine::Max_Pos_X - Ball_Size / 2)
	{
		next_pos_x = CEngine::Max_Pos_X - (next_pos_x - CEngine::Max_Pos_X);
		Ball_Direction = M_PI - Ball_Direction;
	}

	if (next_pos_y > CEngine::Max_Pos_Y)
	{
		next_pos_y = CEngine::Max_Pos_Y - (next_pos_y - CEngine::Max_Pos_Y);
		Ball_Direction = M_PI + (M_PI - Ball_Direction);
	}

	// Корректируем позицию при отражении от платформы
	if (next_pos_y > CPlatform::Pos_Y - Ball_Size / 2)
	{
		if (next_pos_x >= platform->Pos_X && next_pos_x <= platform->Pos_X + platform->Width)
		{
			next_pos_y = CPlatform::Pos_Y - Ball_Size;
			Ball_Direction = M_PI + (M_PI - Ball_Direction);
		}
	}

	// Корректируем позицию при отражении от кирпичей
	level->Check_Level_Brick_Hit(next_pos_y, Ball_Direction);

	// Смещаем шарик
	Ball_Pos_X = next_pos_x;
	Ball_Pos_Y = next_pos_y;

	Ball_Rect.left = Ball_Pos_X * CEngine::Global_Scale;
	Ball_Rect.top = Ball_Pos_Y * CEngine::Global_Scale;
	Ball_Rect.right = Ball_Rect.left + Ball_Size * CEngine::Global_Scale;
	Ball_Rect.bottom = Ball_Rect.top + Ball_Size * CEngine::Global_Scale;

	InvalidateRect(engine->HWnd, &Prev_Ball_Rect, FALSE);
	InvalidateRect(engine->HWnd, &Ball_Rect, FALSE);
}

void CLevel::Init()
{
	Letter_Pen = CreatePen(PS_SOLID, CEngine::Global_Scale, RGB(255, 255, 255));

	CEngine::Create_Pen_Brush(255, 85, 85, Brick_Red_Pen, Brick_Red_Brush);
	CEngine::Create_Pen_Brush(85, 255, 255, Brick_Blue_Pen, Brick_Blue_Brush);

	Level_Rect.left = CLevel::Level_Offset_X * CEngine::Global_Scale;
	Level_Rect.top = CLevel::Level_Offset_X * CEngine::Global_Scale;
	Level_Rect.right = Level_Rect.left + CLevel::Cell_Width * CLevel::Level_Width * CEngine::Global_Scale;
	Level_Rect.bottom = Level_Rect.top + CLevel::Cell_Height * CLevel::Level_Height * CEngine::Global_Scale;
}

// Корректируем позицию при отражении от кирпичей
void CLevel::Check_Level_Brick_Hit(int& next_pos_y, double &ball_direction)
{
	int brick_pos_y = CLevel::Level_Offset_Y + CLevel::Level_Height * Cell_Height;

	for (int i = CLevel::Level_Height - 1; i >= 0; i--)
	{
		for (int j = 0; j < CLevel::Level_Width; j++)
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

		brick_pos_y -= Cell_Height;
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

	int rect_left = x * CEngine::Global_Scale;
	int rect_top = y * CEngine::Global_Scale;
	int rect_right = (x + Brick_Width) * CEngine::Global_Scale;
	int rect_bottom = (y + Brick_Height) * CEngine::Global_Scale;
	int round_width = 2 * CEngine::Global_Scale;
	int round_height = 2 * CEngine::Global_Scale;

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
	int brick_half_height = Brick_Height * CEngine::Global_Scale / 2;
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
			y + brick_half_height - CEngine::Global_Scale,
			x + Brick_Width * CEngine::Global_Scale,
			y + brick_half_height
		);

		// Выводим передний план
		SelectObject(hdc, front_pen);
		SelectObject(hdc, front_brush);

		Rectangle(
			hdc,
			x,
			y + brick_half_height,
			x + Brick_Width * CEngine::Global_Scale,
			y + brick_half_height + CEngine::Global_Scale - 1
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

		offset = 3.0 * (1.0 - fabs(xform.eM22)) * (double)CEngine::Global_Scale;
		back_part_offset = (int)round(offset);
		Rectangle(
			hdc,
			0,
			-brick_half_height - back_part_offset,
			Brick_Width * CEngine::Global_Scale,
			brick_half_height - back_part_offset
		);

		// Выводим передний план
		SelectObject(hdc, front_pen);
		SelectObject(hdc, front_brush);

		Rectangle(hdc, 0, -brick_half_height, Brick_Width * CEngine::Global_Scale, brick_half_height);

		if (rotation_step > 4 && rotation_step <= 12)
		{
			if (letter_type == ELetter_Type::O)
			{
				SelectObject(hdc, Letter_Pen);
				Ellipse(
					hdc,
					0 + 5 * CEngine::Global_Scale,
					-5 * CEngine::Global_Scale / 2,
					0 + 10 * CEngine::Global_Scale,
					5 * CEngine::Global_Scale / 2
				);
			}
		}

		SetWorldTransform(hdc, &old_xform);
	}
}

// Вывод всех кирпичей уровня
void CLevel::Draw(HDC hdc, RECT &paint_area)
{
	RECT intersection_rect;

	if (!IntersectRect(&intersection_rect, &paint_area, &Level_Rect))
	{
		return;
	}

	for (int i = 0; i < Level_Height; i++)
	{
		for (int j = 0; j < Level_Width; j++)
		{
			Draw_Brick(hdc, Level_Offset_X + j * Cell_Width, Level_Offset_Y + i * Cell_Height, (EBrick_Type)Level_01[i][j]);
		}
	}
}

CPlatform::CPlatform()
	: Inner_Width(21), Pos_X(CBorder::Border_Offset_X), X_Step(CEngine::Global_Scale * 2), Width(28)
{}

void CPlatform::Init()
{
	Highlight_Pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));

	CEngine::Create_Pen_Brush(151, 0, 0, Platform_Circle_Pen, Platform_Circle_Brush);
	CEngine::Create_Pen_Brush(0, 128, 192, Platform_Inner_Pen, Platform_Inner_Brush);
}

void CPlatform::Redraw_Platform(CEngine *engine)
{
	Prev_Platform_Rect = Platform_Rect;

	Platform_Rect.left = Pos_X * CEngine::Global_Scale;
	Platform_Rect.top = Pos_Y * CEngine::Global_Scale;
	Platform_Rect.right = Platform_Rect.left + Width * CEngine::Global_Scale;
	Platform_Rect.bottom = Platform_Rect.top + Height * CEngine::Global_Scale;

	InvalidateRect(engine->HWnd, &Prev_Platform_Rect, FALSE);
	InvalidateRect(engine->HWnd, &Platform_Rect, FALSE);
}

// Отрисовка платформы
void CPlatform::Draw(HDC hdc, CEngine* engine, RECT &paint_area)
{
	RECT intersection_rect;

	if (!IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))
	{
		return;
	}

	SelectObject(hdc, engine->BG_Pen);
	SelectObject(hdc, engine->BG_Brush);

	Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);

	// 1. Рисуем боковые шарики платформы
	SelectObject(hdc, Platform_Circle_Pen);
	SelectObject(hdc, Platform_Circle_Brush);

	int x = Pos_X, y = Pos_Y;

	Ellipse(
		hdc,
		x * CEngine::Global_Scale,
		y * CEngine::Global_Scale,
		(x + Circle_Size) * CEngine::Global_Scale,
		(y + Circle_Size) * CEngine::Global_Scale
	);
	Ellipse(
		hdc,
		(x + Inner_Width) * CEngine::Global_Scale,
		y * CEngine::Global_Scale,
		(x + Circle_Size + Inner_Width) * CEngine::Global_Scale,
		(y + Circle_Size) * CEngine::Global_Scale
	);

	// 2. Рисуем блик на левом шарике
	SelectObject(hdc, Highlight_Pen);

	Arc(
		hdc,
		(x + 1) * CEngine::Global_Scale,
		(y + 1) * CEngine::Global_Scale,
		(x + Circle_Size - 1) * CEngine::Global_Scale,
		(y + Circle_Size - 1) * CEngine::Global_Scale,
		(x + 1 + 1) * CEngine::Global_Scale,
		(y + 1) * CEngine::Global_Scale,
		(x + 1) * CEngine::Global_Scale,
		(y + 1 + 2) * CEngine::Global_Scale
	);

	// 3. Рисуем основание платформы
	SelectObject(hdc, Platform_Inner_Pen);
	SelectObject(hdc, Platform_Inner_Brush);

	RoundRect(
		hdc,
		(x + 4) * CEngine::Global_Scale,
		(y + 1) * CEngine::Global_Scale,
		(x + 4 + Inner_Width - 1) * CEngine::Global_Scale,
		(y + 1 + 5) * CEngine::Global_Scale, 3 * CEngine::Global_Scale,
		3 * CEngine::Global_Scale
	);
}

void CBorder::Init()
{
	CEngine::Create_Pen_Brush(85, 255, 255, Border_Blue_Pen, Border_Blue_Brush);
	CEngine::Create_Pen_Brush(255, 255, 255, Border_White_Pen, Border_White_Brush);
}

// Отрисовка элемент рамки уровня
void CBorder::Draw_Element(HDC hdc, int x, int y, bool is_top_border, CEngine *engine)
{
	// Основная линия
	SelectObject(hdc, Border_Blue_Pen);
	SelectObject(hdc, Border_Blue_Brush);

	if (is_top_border)
	{
		Rectangle(hdc,
			x * CEngine::Global_Scale,
			(y + 1) * CEngine::Global_Scale,
			(x + 4) * CEngine::Global_Scale,
			(y + 4) * CEngine::Global_Scale
		);
	}
	else
	{
		Rectangle(hdc,
			(x + 1) * CEngine::Global_Scale,
			y * CEngine::Global_Scale,
			(x + 4) * CEngine::Global_Scale,
			(y + 4) * CEngine::Global_Scale
		);
	}

	// Белая кайма
	SelectObject(hdc, Border_White_Pen);
	SelectObject(hdc, Border_White_Brush);

	if (is_top_border)
	{
		Rectangle(hdc,
			x * CEngine::Global_Scale,
			y * CEngine::Global_Scale,
			(x + 4) * CEngine::Global_Scale,
			(y + 1) * CEngine::Global_Scale
		);
	}
	else
	{
		Rectangle(hdc,
			x * CEngine::Global_Scale,
			y * CEngine::Global_Scale,
			(x + 1) * CEngine::Global_Scale,
			(y + 4) * CEngine::Global_Scale
		);
	}

	// Перфорация
	SelectObject(hdc, engine->BG_Pen);
	SelectObject(hdc, engine->BG_Brush);

	if (is_top_border)
	{
		Rectangle(
			hdc,
			(x + 2) * CEngine::Global_Scale,
			(y + 2) * CEngine::Global_Scale,
			(x + 3) * CEngine::Global_Scale,
			(y + 3) * CEngine::Global_Scale
		);
	}
	else
	{
		Rectangle(hdc,
			(x + 2) * CEngine::Global_Scale,
			(y + 1) * CEngine::Global_Scale,
			(x + 3) * CEngine::Global_Scale,
			(y + 2) * CEngine::Global_Scale
		);
	}
}

// Отрисовка рамки уровня
void CBorder::Draw(HDC hdc, RECT& paint_area, CEngine *engine)
{
	// Левая рамка
	for (int i = 0; i < 50; i++)
	{
		Draw_Element(hdc, 2, 1 + i * 4, false, engine);
	}

	// Правая рамка
	for (int i = 0; i < 50; i++)
	{
		Draw_Element(hdc, 201, 1 + i * 4, false, engine);
	}

	// Верхняя рамка
	for (int i = 0; i < 50; i++)
	{
		Draw_Element(hdc, 3 + i * 4, 0, true, engine);
	}
}

CEngine::CEngine()
{}

// Настройка игры при старте
void CEngine::Init_Engine(HWND hWnd)
{
	HWnd = hWnd;

	Create_Pen_Brush(15, 63, 31, BG_Pen, BG_Brush);

	Ball.Init();
	Level.Init();
	Platform.Init();
	Border.Init();

	Platform.Redraw_Platform(this);

	SetTimer(HWnd, Timer_ID, 20, 0);
}

// Отрисовка экрана игры
void CEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{
	Level.Draw(hdc, paint_area);

	Platform.Draw(hdc, this, paint_area);

	/*for (int i = 0; i < 16; i++)
	{
		Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 100, EBrick_Type::Blue, ELetter_Type::O, i);
		Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 130, EBrick_Type::Red, ELetter_Type::O, i);
	}*/

	Ball.Draw(hdc, paint_area, this);

	Border.Draw(hdc, paint_area, this);
}

int CEngine::On_Key_Down(EKey_Type key_type)
{
	switch (key_type)
	{
	case EKey_Type::Left:
		Platform.Pos_X -= Platform.X_Step;

		if (Platform.Pos_X <= CBorder::Border_Offset_X)
		{
			Platform.Pos_X = CBorder::Border_Offset_X;
		}

		Platform.Redraw_Platform(this);
		break;

	case EKey_Type::Right:
		Platform.Pos_X += Platform.X_Step;

		if (Platform.Pos_X >= Max_Pos_X - Platform.Width + 1)
		{
			Platform.Pos_X = Max_Pos_X - Platform.Width + 1;
		}

		Platform.Redraw_Platform(this);
		break;

	case EKey_Type::Space:
		break;
	}

	return 0;
}

int CEngine::On_Timer()
{
	Ball.Move(this, &Level, &Platform);

	return 0;
}

void CEngine::Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush)
{
	pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
	brush = CreateSolidBrush(RGB(r, g, b));
}