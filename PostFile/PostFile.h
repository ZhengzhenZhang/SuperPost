
// PostFile.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPostFileApp:
// �йش����ʵ�֣������ PostFile.cpp
//

class CPostFileApp : public CWinApp
{
public:
	CPostFileApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPostFileApp theApp;