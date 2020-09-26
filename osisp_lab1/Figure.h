#pragma once

#include "framework.h"
#include <windowsx.h>
#include <string>

enum FigureType {
	ftRectangle, ftEllipse
};

class Figure
{
public:
	FigureType type;
	RECT coords;

	Figure(FigureType type, RECT coords) {
		this->type = type;
		this->coords = coords;
	}
};


/*class fRectangle : public Figure {
public:
	RECT coords;
};

class fEllipse : public Figure {
public:
	RECT coords;
};*/