#pragma once

#include <windowsx.h>
#include <vector>
#include "resource.h"
#include "Figure.h"
#include "RectangleFigure.h"
#include "EllipseFigure.h"

void SwitchFigureType(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void CreateInstance();

void onClickDown(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void onMouseMove(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void onClickUp(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void onPaint(HDC hdc, Figure* figure);
