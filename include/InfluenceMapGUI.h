#pragma once

#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include "stdafx.h"

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <BWAPI.h>
///////////////////////////
//MainDlg.h
///////////////////////////
class InfluenceMapGUI 
{

	//--------------------------------------------------------------------------------------------   
public:
	InfluenceMapGUI(HINSTANCE hInst);
	virtual ~InfluenceMapGUI();
	BOOL CreateMainDlg();

private:
	HINSTANCE m_hInst;
	HWND m_hDlg;

};