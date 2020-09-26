#pragma once

#include "framework.h"
#include "osisp_lab1.h"

enum FigureType {
	ftRectangle, ftEllipse
};

class Figure {

public:
	FigureType type;

	virtual void Paint(HDC hdc) {}
	virtual void ClickDown(int x1, int y1, int x2, int y2) {}
	virtual void MouseMove(int x1, int y1) {}
	virtual void ClickUp(int x1, int y1, std::vector<Figure*>& FigureVector) {}
};