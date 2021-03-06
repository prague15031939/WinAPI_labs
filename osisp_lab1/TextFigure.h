#pragma once

#include "Figure.h"
#include "framework.h"
#include <ctime>

class TextFigure : public Figure {

public:
	int xStart;
	int yStart;
	BOOL isTyping;
	std::wstring text = L"";

	void Paint(HDC hdc) override {
		HFONT hFont = CreateFont(24, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
			CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Gill Sans MT"));
		SelectObject(hdc, hFont);

		SetTextColor(hdc, RGB(0, 64, 0));

		time_t tm = time(NULL);
		if ((tm % 2) == 0 && isTyping) {
			text.push_back('|');
			TextOut(hdc, xStart, yStart, text.c_str(), wcslen(text.c_str()));
			text.pop_back();
		}
		else
			TextOut(hdc, xStart, yStart, text.c_str(), wcslen(text.c_str()));
	}

	void ModifyText(WPARAM wParam) {
		if (isTyping) {
			UINT c = MapVirtualKeyA(wParam, MAPVK_VK_TO_CHAR);
			if (c != 0) {
				if (c == '\b' && text.size() != 0)
					text.pop_back();
				else {
					if (GetKeyState(VK_SHIFT) & 0x8000)
						text.push_back(c);
					else
						text.push_back(tolower(c));
				}
			}
		}
	}

	void ClickDown(int x1, int y1, int x2, int y2) override {
		xStart = x1;
		yStart = y1;
		isTyping = true;
	}

	void FinishText(std::vector<Figure*>& FigureVector) {
		if (isTyping && text.size() != 0) {
			isTyping = false;
			FigureVector.push_back(this);
		}
	}
};