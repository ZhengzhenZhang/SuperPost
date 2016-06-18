// MySocket.cpp : 实现文件
//

#include "stdafx.h"
#include "PostFile.h"
#include "MyTask.h"
#include "PostFileDlg.h"


// CMyTask

CMyTask::CMyTask()
{
	CPostFileDlg* pDlg = NULL;

	m_strPath = _T("");
	m_strAddress = _T("");
	m_nPort = 0;
	m_nSleep = 0;
	m_nPerSize = 0;

	m_nIndex = 0;

	m_bSuspend = false;
	m_bStop = true;
	m_bLoop = false;
	m_llPrgs = 0;		// 进度
}



CMyTask::~CMyTask()
{
	StopTask();
}

CCriticalSection g_cs;	// 防止线程初始化时候发生冲突
UINT TreadSend(LPVOID pParam)
{
	CMyTask *pTask = (CMyTask*)pParam;
	pTask->m_csFinish.Lock();

	g_cs.Lock();

	int nIndex = pTask->m_nIndex;
	CString strPath = pTask->m_strPath;
	CString strAddress = pTask->m_strAddress;
	int nPort = pTask->m_nPort;
	int nSleep = pTask->m_nSleep;
	int nPerSize = pTask->m_nPerSize;
	CPostFileDlg* pDlg = pTask->m_pDlg;

	bool bFailed = false;
	if (!AfxSocketInit())
	{
		AfxMessageBox(_T("Socket初始化失败！"));
		bFailed = true;
	}

	if (!pTask->Create(0, SOCK_DGRAM))
	{
		AfxMessageBox(_T("Socket创建失败！"));
		bFailed = true;
	}

	CFile file;
	if (!file.Open(strPath, CFile::modeRead | CFile::shareDenyNone))
	{
		strPath = _T("打开文件失败!\n") + strPath;
		AfxMessageBox(strPath);
		bFailed = true;
	}

	g_cs.Unlock();
	
	if (bFailed)
	{
		pTask->m_csFinish.Unlock();

		file.Abort();
		pTask->StopTask();
		return 0;
	}

	ULONGLONG llTotalSize;
	llTotalSize = file.GetLength();
	nPerSize = nPerSize > (int)llTotalSize ? (int)llTotalSize : nPerSize;

	do 
	{
		file.SeekToBegin();
		BYTE buf[BUFSIZE];
		ULONGLONG llNow = 0;
		pTask->m_llPrgs = 0;
		while (llNow < llTotalSize && !pTask->m_bStop)
		{
			if (pTask->m_bSuspend)
			{
				Sleep(500);
				continue;
			}
			llNow += nPerSize;
			file.Read(buf, nPerSize);
			pTask->SendTo(buf, nPerSize, nPort, strAddress);


			int nTmpPrgrs = (int)(llNow*1.0 / llTotalSize * 100 + 0.5);
			nTmpPrgrs = nTmpPrgrs > 100 ? 100 : nTmpPrgrs;

			if (nTmpPrgrs > pTask->m_llPrgs)
				pTask->m_llPrgs = nTmpPrgrs;

			Sleep(nSleep);
		}
	} while (pTask->m_bLoop && !pTask->m_bStop);



	file.Abort();
	pTask->Close();

	pTask->m_csFinish.Unlock();
	return 0;
}


// CMyTask 成员函数

void CMyTask::OnReceive(int nErrorCode)
{
	if (m_bSuspend)
		return;

	BYTE recBuf[BUFSIZE];
	int len = Receive(recBuf, sizeof (recBuf));

	m_file.Write(recBuf, len);

	m_llPrgs += len;

	CAsyncSocket::OnReceive(nErrorCode);
}


void CMyTask::BeginTask()
{
	if (m_strAddress != _T("###"))
	{
		int nImage = m_bLoop ? 3 : 0;

		(m_pDlg->m_listTask).SetItem(m_nIndex, 0, LVIF_IMAGE, NULL, nImage,  NULL, NULL, NULL);

		if (m_bStop)
		{
			m_bSuspend = false;
			m_bStop = false;

			CString strTmp;
			strTmp.Format(_T("0 %s"), _T("%"));
			(m_pDlg->m_listTask).SetItemText(m_nIndex, 7, strTmp);

			AfxBeginThread(TreadSend, this);
		}
		else if (m_bSuspend)
			m_bSuspend = false;
	}
	else
	{
		(m_pDlg->m_listTask).SetItem(m_nIndex, 0, LVIF_IMAGE, NULL, 0,  NULL, NULL, NULL);

		if (m_bStop)
		{
			m_bSuspend = false;
			m_bStop = false;

			CString strTmp;
			strTmp.Format(_T("0 %s"), _T("bs"));
			(m_pDlg->m_listTask).SetItemText(m_nIndex, 7, strTmp);

			bool bFailed = false;
			if (!m_file.Open(m_strPath, CFile::modeReadWrite | CFile::modeCreate))
			{
				AfxMessageBox(_T("创建接收文件失败！"));
				bFailed = true;
			}
			else
			{
				if (!AfxSocketInit())
				{
					AfxMessageBox(_T("Socket初始化失败！"));
					bFailed = true;
				}
				if (!Create(m_nPort, SOCK_DGRAM))
				{
					AfxMessageBox(_T("Socket创建失败！"));
					bFailed = true;
				}
			}

			if (bFailed)
				StopTask();
		}
		else if (m_bSuspend)
			m_bSuspend = false;
	}
}


void CMyTask::StopTask()
{
	(m_pDlg->m_listTask).SetItem(m_nIndex, 0, LVIF_IMAGE, NULL, 2,  NULL, NULL, NULL);
	m_bStop = true;
	m_bSuspend = true;
	m_bLoop= false;
	m_llPrgs = 0;

	// 等待线程结束
	m_csFinish.Lock();
	m_csFinish.Unlock();

	m_file.Abort();
	this->Close();
}


void CMyTask::SuspendTask()
{
	if (m_bStop)
		return;
	(m_pDlg->m_listTask).SetItem(m_nIndex, 0, LVIF_IMAGE, NULL, 1,  NULL, NULL, NULL);
	m_bSuspend = true;
}