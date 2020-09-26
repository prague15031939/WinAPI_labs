#pragma once

#include "resource.h"
#include "Figure.h"
#include <windowsx.h>

BOOL ClickDownFunction(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL PullFunction(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL ClickUpFunction(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL SwitchFigureTypeFunction(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
