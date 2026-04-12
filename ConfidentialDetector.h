
// ConfidentialDetector.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CConfidentialDetectorApp:
// Сведения о реализации этого класса: ConfidentialDetector.cpp
//

class CConfidentialDetectorApp : public CWinApp
{
public:
	CConfidentialDetectorApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CConfidentialDetectorApp theApp;
