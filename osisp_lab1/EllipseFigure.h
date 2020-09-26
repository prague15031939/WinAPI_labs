#pragma once

#include "Figure.h"
#include "framework.h"

class EllipseFigure : public Figure {

public:
	RECT coords;

	void Paint(HDC hdc) override {
		Ellipse(hdc, coords.left, coords.top, coords.right, coords.bottom);
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
		FigureVector.push_back(this);
	}
};