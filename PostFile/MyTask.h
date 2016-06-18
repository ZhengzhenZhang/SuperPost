#pragma once

// CMyTask 命令目标
#define BUFSIZE 1000005		// 缓冲区大小

class CPostFileDlg;
class CMyTask : public CAsyncSocket
{
public:
	CMyTask();
	virtual ~CMyTask();
	virtual void OnReceive(int nErrorCode);

public:
	void BeginTask();
	void StopTask();
	void SuspendTask();

public:
	CPostFileDlg* m_pDlg;
	CFile m_file;

	CString m_strPath;
	CString m_strAddress;
	int m_nPort;
	int m_nSleep;
	int m_nPerSize;
	
	int m_nIndex;

	bool m_bSuspend;
	bool m_bStop;
	bool m_bLoop;
	LONGLONG m_llPrgs;		// 进度

public:
	CCriticalSection m_csFinish;
};


