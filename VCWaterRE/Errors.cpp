#include "Errors.h"
#include <windows.h>
#include <stdio.h>
#include <signal.h>
#include "_precl.h"
#include "_config.h"

// based on https://git.nick7.com/public/x-gtasa

void FH_Assert(eAssertType as, const char *file, int line, const char *func, const char *expr, const char *msg, ...)
{
	va_list ap;

	char buff[4096];
	char massageBuff[2048];
	char boxtitle[512];
	
	sprintf_s(boxtitle, "%s(%s): Shit Happens!\n", PROJECT_NAME, __TIMESTAMP__);
	
	if (msg) {
		va_start(ap, msg);
		vsprintf_s(massageBuff, msg, ap);
	}
	
	if ( as == ASSTYPE_ASSERT )
		sprintf_s(buff, "Assertion Failed!\n\n");
	else if ( as == ASSTYPE_ERROR )
		sprintf_s(buff, "Error!\n\n");
	else if ( as == ASSTYPE_FATAL_ERROR )
		sprintf_s(buff, "Fatal Error!\n\n");
	
	if (msg)
		sprintf_s(buff + strlen(buff), 4096 - strlen(buff), "%s\n\n", massageBuff);

	if ( as == ASSTYPE_ASSERT || as == ASSTYPE_ERROR )
	{
		if (expr)
			sprintf_s(buff + strlen(buff), 4096 - strlen(buff), "Expression:\n   %s\n\n", expr);
	
		if (file)
			sprintf_s(buff + strlen(buff), 4096 - strlen(buff), "File: %s\nLine: %d\n", file, line);
	
		if (func)
			sprintf_s(buff + strlen(buff), 4096 - strlen(buff), "Function: %s\n", func);
	
	}

	if (::IsDebuggerPresent())
		strcat_s(buff, "\n(Press OK to debug application)");
	else
		strcat_s(buff, "\n(Press OK to exit application)");
	
	if ( as == ASSTYPE_ASSERT || as == ASSTYPE_ERROR )
		strcat_s(buff, "\n(Press CANCEL to skip error)");

	
	HWND hActive = ::GetActiveWindow();
	//HWND hActive = PSGLOBAL(window);
	bool bHideMouseAgain = false;
	if (hActive)
	{
		::ShowWindow(hActive, SW_HIDE);
		::SetCursor(::LoadCursor(NULL, IDC_ARROW));
		bHideMouseAgain = ::ShowCursor(true) <= 0;
		while (::ShowCursor(true) < 0); // force to show mouse
	}
	
	DWORD flags = MB_ICONERROR;

	if ( as == ASSTYPE_ASSERT || as == ASSTYPE_ERROR )
		flags = flags | MB_OKCANCEL;
	else
		flags = flags | MB_OK;

	int msgbox = MessageBoxA(NULL, buff, boxtitle, flags);
	
	switch (msgbox)
	{
		case IDOK:
			if (::IsDebuggerPresent())
				DebuggerBreak();
			else
				raise(SIGABRT);
			break;
		
		case IDCANCEL:
			if (hActive)
			{
				if (bHideMouseAgain)
					while (::ShowCursor(false) >= 0); // force to hide mouse
				::ShowWindow(hActive, SW_SHOW);
			}
			break;
	}
}