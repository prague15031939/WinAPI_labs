#pragma once

#include "Figure.h"
#include "framework.h"

class PolylineFigure : public Figure {

public:
	std::vector<RECT*> coords;
	RECT* CurrentLine;
	BOOL isDrawing = false;
	BOOL isPoly = false;

	void Paint(HDC hdc) override {
		for (int i = 0; i < coords.size(); i++) {
			MoveToEx(hdc, coords[i]->left, coords[i]->top, NULL);
			LineTo(hdc, coords[i]->right, coords[i]->bottom);
		}
		if (isDrawing)
		{
			MoveToEx(hdc, CurrentLine->left, CurrentLine->top, NULL);
			LineTo(hdc, CurrentLine->right, CurrentLine->bottom);
		}
	}

	void ClickDown(int x1, int y1, int x2, int y2) override {
		CurrentLine = new RECT();
		isDrawing = true;
		CurrentLine->left = x1;
		CurrentLine->top = y1;
		CurrentLine->right = x2;
		CurrentLine->bottom = y2;
	}

	void MouseMove(int x1, int y1) override {
		CurrentLine->right = x1;
		CurrentLine->bottom = y1;
	}

	void ClickUp(int x1, int y1, std::vector<Figure*>& FigureVector) override {
		if (isPoly) {
			CurrentLine->right = x1;
			CurrentLine->bottom = y1;
			coords.push_back(CurrentLine);
		}
		isDrawing = false;
		if (coords.size() == 0) return;
		if (coords[0]->left == coords[0]->right && coords[0]->top == coords[0]->bottom) return;
		FigureVector.push_back(this);
	}

	// adds a node in polyline (not a finish one, only central)
	void RClickDown(int x1, int y1) {
		if (isDrawing) {
			CurrentLine->right = x1;
			CurrentLine->bottom = y1;
			if (!(CurrentLine->left == CurrentLine->right && CurrentLine->top == CurrentLine->bottom)) {
				coords.push_back(CurrentLine);
				isPoly = true;
			}
			ClickDown(x1, y1, x1, y1);
		}
	}
};