#pragma once

#include "Figure.h"
#include "framework.h"
#include <cmath>

class PolygonFigure : public Figure {

public:
	RECT coords;

	void Paint(HDC hdc) override {

		int height = abs(coords.top - coords.bottom);
		int width = abs(coords.right - coords.left);

		MoveToEx(hdc, coords.left, coords.top + height / 3, NULL);
		LineTo(hdc, coords.left, coords.top + 2 * height / 3);
		LineTo(hdc, coords.left + width / 3, coords.bottom);
		LineTo(hdc, coords.left + 2 * width / 3, coords.bottom);
		LineTo(hdc, coords.right, coords.top + 2 * height / 3);
		LineTo(hdc, coords.right, coords.top + height / 3);
		LineTo(hdc, coords.left + 2 * width / 3, coords.top);
		LineTo(hdc, coords.left + width / 3, coords.top);
		LineTo(hdc, coords.left, coords.top + height / 3);
	}

	void ClickDown(int x1, int y1, int x2, int y2) override {
		coords.left = x1;
		coords.top = y1;
		coords.right = x2;
		coords.bottom = y2;
	}

	void MouseMove(int x1, int y1) override {
		coords.right = x1;
		coords.bottom = y1;
	}

	void ClickUp(int x1, int y1, std::vector<Figure*>& FigureVector) override {
		coords.right = x1;
		coords.bottom = y1;
		if (coords.left == coords.right && coords.top == coords.bottom) return;
		FigureVector.push_back(this);
	}
};
