#pragma once
#include <Windows.h>
#include "header.h"

HHOOK mouse_hook;

bool uped = true;
bool downed = false;

LONG oldWndProc;

LRESULT CALLBACK MouseHook(_In_ int nCode, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	if (nCode == 0 || TRUE)
	{
		if(wParam == WM_LBUTTONDOWN)
		{
			downed = true;
			if (functions.aimbot && !CursorShowing() && uped)
			{
				if (GetNumberOfShoots() == 0)
				{
					RageAim(false);
				//	RageAim(true);
				}
			}
			//uped = false;
		}
		else if(wParam == WM_LBUTTONUP)
		{
			uped = true;
			downed = false;
		}
	}
    
	return CallNextHookEx(mouse_hook, nCode, wParam, lParam);
}


LRESULT HookWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM  lParam)
{
	if(wParam == WM_LBUTTONDOWN)
	{
		RageAim();
	}

	return CallWindowProcW(reinterpret_cast<WNDPROC>(oldWndProc), hWnd, Msg, wParam, lParam);
}

BOOL SetWndProcHook()
{
	oldWndProc = SetWindowLongW(csWnd, GWL_WNDPROC, (LONG)HookWindowProc);
	return oldWndProc;
}

