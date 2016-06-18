
// PostFileDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PostFile.h"
#include "PostFileDlg.h"
#include "afxdialogex.h"
#include "DlgTask.h"
#include "DlgDeal.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPostFileDlg �Ի���



CPostFileDlg::CPostFileDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPostFileDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_strDragPath = _T("");
	m_strDirectory = _T("");
}

void CPostFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SEND, m_listTask);
	DDX_Control(pDX, IDC_PROGRESS_TASK, m_prgsTask);
}

BEGIN_MESSAGE_MAP(CPostFileDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_ADDTASK, &CPostFileDlg::OnBnClickedButtonAddtask)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SEND, &CPostFileDlg::OnNMDblclkListTask)
	ON_WM_DROPFILES()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_MENU_BEGIN, &CPostFileDlg::OnMenuBegin)
	ON_COMMAND(ID_MENU_SUSPEND, &CPostFileDlg::OnMenuSuspend)
	ON_COMMAND(ID_MENU_STOP, &CPostFileDlg::OnMenuStop)
	ON_COMMAND(ID_MENU_DELETE, &CPostFileDlg::OnMenuDelete)
	ON_COMMAND(ID_MENU_LOOP, &CPostFileDlg::OnMenuLoop)
	ON_COMMAND(ID_OTHER_SELALL, &CPostFileDlg::OnOtherSelall)
	ON_COMMAND(ID_OTHER_SPACE, &CPostFileDlg::OnOtherSpace)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_BEGIN, &CPostFileDlg::OnMenuBegin)
	ON_BN_CLICKED(IDC_BUTTON_SUSPEND, &CPostFileDlg::OnMenuSuspend)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CPostFileDlg::OnMenuStop)
	ON_BN_CLICKED(IDC_BUTTON_LOOP, &CPostFileDlg::OnMenuLoop)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CPostFileDlg::OnMenuDelete)
	ON_COMMAND(ID_MENU_MODIFY, &CPostFileDlg::OnMenuModify)
	ON_BN_CLICKED(IDC_BUTTON_DEAL, &CPostFileDlg::OnBnClickedButtonDeal)
END_MESSAGE_MAP()


// CPostFileDlg ��Ϣ�������



BOOL CPostFileDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	// �����б���ͼ����
	m_ilList.Create(IDB_BITMAP_LIST, 16, 1, RGB(255, 0, 255));
	m_listTask.SetExtendedStyle(m_listTask.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	m_listTask.SetImageList(&m_ilList, LVSIL_SMALL);
	
	int nIndex = 0;
	m_listTask.InsertColumn(nIndex++, _T(""), LVCFMT_LEFT, 25);
	m_listTask.InsertColumn(nIndex++, _T(""), LVCFMT_LEFT, 26);
	m_listTask.InsertColumn(nIndex++, _T("�ļ���"), LVCFMT_LEFT, 150);
	m_listTask.InsertColumn(nIndex++, _T("Ŀ��IP"), LVCFMT_LEFT, 120);
	m_listTask.InsertColumn(nIndex++, _T("�˿ں�"), LVCFMT_LEFT, 65);
	m_listTask.InsertColumn(nIndex++, _T("���ͼ��"), LVCFMT_RIGHT, 65);
	m_listTask.InsertColumn(nIndex++, _T("����С"), LVCFMT_RIGHT, 65);
	m_listTask.InsertColumn(nIndex++, _T("����"), LVCFMT_RIGHT, 80);
	m_listTask.InsertColumn(nIndex++, _T("·��"), LVCFMT_LEFT, 500);

	// ������ק�ļ�
	DragAcceptFiles(TRUE);

	// ������ʱ��
	SetTimer(1, TIMER1, NULL);

	// ���ؿ�ݼ�
	m_hAccelTable = ::LoadAccelerators(AfxGetInstanceHandle(), 
		MAKEINTRESOURCE(IDR_ACCELERATORS));

	// ����������ʾ
	m_ctlTT.Create(this);
	m_ctlTT.AddWindowTool(GetDlgItem(IDC_BUTTON_BEGIN),
		MAKEINTRESOURCE(IDS_BUTTON_BEGIN));
	m_ctlTT.AddWindowTool(GetDlgItem(IDC_BUTTON_SUSPEND),
		MAKEINTRESOURCE(IDS_BUTTON_SUSPEND));
	m_ctlTT.AddWindowTool(GetDlgItem(IDC_BUTTON_STOP),
		MAKEINTRESOURCE(IDS_BUTTON_STOP));
	m_ctlTT.AddWindowTool(GetDlgItem(IDC_BUTTON_LOOP),
		MAKEINTRESOURCE(IDS_BUTTON_LOOP));
	m_ctlTT.AddWindowTool(GetDlgItem(IDC_BUTTON_DELETE),
		MAKEINTRESOURCE(IDS_BUTTON_DELETE));
	m_ctlTT.AddWindowTool(GetDlgItem(IDC_BUTTON_DEAL),
		MAKEINTRESOURCE(IDS_BUTTON_DEAL));

	// ���ð�ťͼ��
	((CButton*)GetDlgItem(IDC_BUTTON_BEGIN))->SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_BEGIN));
	((CButton*)GetDlgItem(IDC_BUTTON_STOP))->SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_STOP));
	((CButton*)GetDlgItem(IDC_BUTTON_SUSPEND))->SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_SUSPEND));
	((CButton*)GetDlgItem(IDC_BUTTON_LOOP))->SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_LOOP));
	((CButton*)GetDlgItem(IDC_BUTTON_DELETE))->SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_DELETE));
	((CButton*)GetDlgItem(IDC_BUTTON_DEAL))->SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_DEAL));

	// �õ���ǰ�������Ŀ¼
	TCHAR tcPath[MAX_PATH];
	::GetCurrentDirectory(sizeof (tcPath), tcPath);
	m_strDirectory = tcPath;

	// ��ȡ��������б�
	ReadRecently();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CPostFileDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPostFileDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CPostFileDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPostFileDlg::ReadRecently()
{
	CStdioFile stdFile;
	CString strPath = m_strDirectory + _T("\\SuperPost\\config_list.rct");
	if (stdFile.Open(strPath, CFile::modeRead))
	{
		CString strRead;
		int nIndex = 0;
		while (stdFile.ReadString(strRead))
		{
			int nIndex = m_listTask.GetItemCount();
			m_listTask.InsertItem(nIndex, _T(""), 2);
			m_listTask.SetItemText(nIndex, 2, strRead);

			bool bSend;
			for (int i = 3; i < 9; i++)
			{
				if (i == 7)
					continue;
				stdFile.ReadString(strRead);
				m_listTask.SetItemText(nIndex, i, strRead);

				if (3 == i)
					bSend = strRead != _T("###");
			}

			CString strTmp;
			strTmp.Format(_T("0 %s"), bSend ? _T("%") : _T("bs"));
			m_listTask.SetItemText(nIndex, 7, strTmp);

			AddTask(nIndex);
		}

		stdFile.Close();

		UpdateListNum();
	}

}


void CPostFileDlg::WriteRecently()
{ 

	CStdioFile stdFile;

	CString strPath = m_strDirectory;
	strPath += _T("\\SuperPost");

	::CreateDirectory(strPath, NULL);

	strPath += _T("\\config_list.rct");

	if (stdFile.Open(strPath, CFile::modeWrite | CFile::modeCreate))
	{

		int nCounts = m_listTask.GetItemCount();
		CString strWrite;
		 
		for (int i = 0; i < nCounts; i++)
		{
			for (int j = 2; j < 9; j++)
			{
				if (j == 7)
					continue;
				strWrite = m_listTask.GetItemText(i, j) + _T('\n');
				stdFile.WriteString(strWrite);
			}
		}

		nCounts = m_listTask.GetItemCount();
		stdFile.Close();
	}
}


void CPostFileDlg::OnBnClickedButtonAddtask()
{
	
	CDlgTask dlg;
	dlg.m_strPath = m_strDragPath;
	dlg.m_strDirectory = m_strDirectory;
	if (IDCANCEL == dlg.DoModal())
		return;

	CString strPath;
	CString strName;
	
	strName = dlg.m_strName;
	strPath = dlg.m_strPath;


	int nIndex = 0;

	nIndex = m_listTask.GetItemCount();

	int nSubItem = 2;
	m_listTask.InsertItem(nIndex, _T(""), 2);

	m_listTask.SetItemText(nIndex, nSubItem++, strName);

	for (int i = 0; i < 4; i++)
		m_listTask.SetItemText(nIndex, nSubItem++, dlg.m_strDIPP[i]);

	CString strTmp;
	strTmp.Format(_T("0 %s"), dlg.m_bReceive ? _T("bs") : _T("%"));
	m_listTask.SetItemText(nIndex, nSubItem++, strTmp);
	m_listTask.SetItemText(nIndex, nSubItem++, strPath);

	AddTask(nIndex);
	UpdateListNum();
}


void CPostFileDlg::AddTask(int nIndex)
{
	CMyTask *pTask = new CMyTask;

	pTask->m_pDlg = this;
	pTask->m_nIndex = nIndex;
	pTask->m_strAddress = m_listTask.GetItemText(nIndex, 3);
	pTask->m_nPort = _ttoi(m_listTask.GetItemText(nIndex, 4));
	pTask->m_nSleep = _ttoi(m_listTask.GetItemText(nIndex, 5));
	pTask->m_nPerSize = _ttoi(m_listTask.GetItemText(nIndex, 6));
	pTask->m_strPath = m_listTask.GetItemText(nIndex, 8);

	m_listTask.SetItemData(nIndex, (DWORD_PTR)pTask);
}


void CPostFileDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	TCHAR tChFile[MAX_PATH];
	DragQueryFile(hDropInfo, 0, tChFile, sizeof (tChFile));

	m_strDragPath = tChFile;
	OnBnClickedButtonAddtask();
	m_strDragPath = _T("");

	CDialogEx::OnDropFiles(hDropInfo);
}


void CPostFileDlg::OnNMDblclkListTask(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	int nIndex = m_listTask.GetSelectionMark();
	if (nIndex >= 0)
	{
		CMyTask *pTask = (CMyTask*)(m_listTask.GetItemData(nIndex));
		
		if (pTask->m_bStop || pTask->m_bSuspend)
			pTask->BeginTask();
		else
			pTask->SuspendTask();
	}

	*pResult = 0;
}


void CPostFileDlg::OnCancel()
{
	if (IDCANCEL == MessageBox(_T("ȷ��Ҫ�˳���"), _T("SuperPost"), 
		MB_ICONQUESTION | MB_OKCANCEL))
		return;

	int nCounts = m_listTask.GetItemCount();

	for (int i = 0; i < nCounts; i++)
	{
		CMyTask *pTask = (CMyTask*)m_listTask.GetItemData(i);
		delete pTask;
	}

	// д����������б�
	WriteRecently();

	CDialogEx::OnCancel();
}


void CPostFileDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_MENU_CONTEXT);
	CMenu *pContextMenu = menu.GetSubMenu(0);

	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON 
		| TPM_RIGHTBUTTON, point.x, point.y, AfxGetMainWnd());
}


void CPostFileDlg::OnMenuBegin()
{
	
	int nIndex = -1;
	while (1)
	{
		nIndex = m_listTask.GetNextItem(nIndex, LVNI_SELECTED);
		if (nIndex == -1)
			break;

		CMyTask *pTask = (CMyTask*)m_listTask.GetItemData(nIndex);
		pTask->BeginTask();
	}
}


void CPostFileDlg::OnMenuSuspend()
{
	int nIndex = -1;
	while (1)
	{
		nIndex = m_listTask.GetNextItem(nIndex, LVNI_SELECTED);
		if (nIndex == -1)
			break;

		CMyTask *pTask = (CMyTask*)m_listTask.GetItemData(nIndex);
		pTask->SuspendTask();
	}
}


void CPostFileDlg::OnMenuStop()
{
	bool bFistStart = true;
	int nIndex = -1;
	while (1)
	{
		nIndex = m_listTask.GetNextItem(nIndex, LVNI_SELECTED);
		if (nIndex == -1)
			break;

		if (bFistStart)
		{
			if (IDCANCEL == MessageBox(_T("ȷ��Ҫֹͣѡ������"), _T("SuperPost"), 
				MB_ICONQUESTION | MB_OKCANCEL))
				return;
			bFistStart = false;
		}

		CMyTask *pTask = (CMyTask*)m_listTask.GetItemData(nIndex);
		pTask->StopTask();
	}
}


void CPostFileDlg::OnMenuDelete()
{
	bool bFistStart = true;
	int nIndex = -1;
	while (1)
	{
		nIndex = m_listTask.GetNextItem(nIndex, LVNI_SELECTED);
		if (nIndex == -1)
			break;

		if (bFistStart)
		{
			if (IDCANCEL == MessageBox(_T("ȷ��Ҫɾ��ѡ������"), _T("SuperPost"), 
				MB_OKCANCEL | MB_ICONQUESTION))
				return;
			bFistStart = false;
		}

		CMyTask *pTask = (CMyTask*)m_listTask.GetItemData(nIndex);
		delete pTask;

		m_listTask.DeleteItem(nIndex--);
	}

	UpdateListNum();
}


void CPostFileDlg::OnMenuLoop()
{
	int nIndex = -1;
	while (1)
	{
		nIndex = m_listTask.GetNextItem(nIndex, LVNI_SELECTED);
		if (nIndex == -1)
			break;

		CMyTask *pTask = (CMyTask*)m_listTask.GetItemData(nIndex);

		if (pTask->m_llPrgs >= 100)
			pTask->StopTask();

		pTask->m_bLoop = true;
		pTask->BeginTask();
	}
}


void CPostFileDlg::OnOtherSelall()
{
	int nCounts = m_listTask.GetItemCount();
	for (int i = 0; i < nCounts; i++)
		m_listTask.SetItemState(i, LVNI_FOCUSED | LVNI_SELECTED, 
		LVNI_FOCUSED | LVNI_SELECTED);
}


void CPostFileDlg::OnOtherSpace()
{
	int nIndex = -1;
	bool bBegin = false;
	while (1)
	{
		nIndex = m_listTask.GetNextItem(nIndex, LVNI_SELECTED);
		if (nIndex == -1)
			break;

		CMyTask *pTask = (CMyTask*)m_listTask.GetItemData(nIndex);

		if (pTask->m_bSuspend || pTask->m_bStop)
		{
			bBegin = true;
			break;
		}
	}

	if (bBegin)
		OnMenuBegin();
	else
		OnMenuSuspend();
}


BOOL CPostFileDlg::PreTranslateMessage(MSG* pMsg)
{
	if (CWnd::PreTranslateMessage(pMsg))
		return TRUE;

	return ((m_hAccelTable != NULL) && 
		::TranslateAccelerator(m_hWnd, m_hAccelTable, pMsg));
}


void CPostFileDlg::OnTimer(UINT_PTR nIDEvent)
{
	CString strTmp;
	int minPrgs = 0xffff;
	int nCounts = m_listTask.GetItemCount();

	for (int i = 0; i < nCounts; i++)
	{
		// �õ���С��������
		CMyTask *pTask = (CMyTask*)m_listTask.GetItemData(i);
		bool bSend = pTask->m_strAddress != _T("###");

		if (bSend && !pTask->m_bStop && minPrgs > pTask->m_llPrgs)
			minPrgs = (int)pTask->m_llPrgs;

		// ˢ���б���ͼ����
		LONGLONG llTmpPrgs = _ttoi(m_listTask.GetItemText(i, 7));

		if (llTmpPrgs != pTask->m_llPrgs)
		{
			strTmp.Format(_T("%lld %s"), pTask->m_llPrgs, bSend ? _T("%") : _T("bs"));
			m_listTask.SetItemText(i, 7, strTmp);
		}
	}

	if (minPrgs == 0xffff)
		m_prgsTask.SetPos(0);
	else
		m_prgsTask.SetPos(minPrgs);

	CDialogEx::OnTimer(nIDEvent);
}


void CPostFileDlg::OnMenuModify()
{
	int nIndex = m_listTask.GetSelectionMark();
	if (nIndex >= 0)
	{
		CMyTask *pTask = (CMyTask*)m_listTask.GetItemData(nIndex);
		if (!pTask->m_bStop)
		{
			if (IDCANCEL == MessageBox(_T("ȷ��Ҫֹͣ�����񲢽����޸ģ�"), _T("SuperPost"), 
				MB_ICONQUESTION | MB_OKCANCEL))
				return;

			pTask->StopTask();
		}

		CDlgTask dlg;
		dlg.m_bModify = true;
		dlg.m_strDirectory = m_strDirectory;
		dlg.m_strPath = m_listTask.GetItemText(nIndex, 8);
		dlg.m_bReceive = (pTask->m_strAddress == _T("###"));

		for (int i = 0; i < 4; i++)
			dlg.m_strDIPP[i] = m_listTask.GetItemText(nIndex, i + 3);

		if (IDCANCEL == dlg.DoModal())
			return;

		for (int i = 0; i < 4; i++)
			m_listTask.SetItemText(nIndex, i + 3, dlg.m_strDIPP[i]);

		delete pTask;
		AddTask(nIndex);
	}
}


void CPostFileDlg::UpdateListNum()
{
	int nCounts = m_listTask.GetItemCount();
	
	CString strTmp;
	for (int i = 0; i < nCounts; i++)
	{
		strTmp.Format(_T("%d"), i + 1);
		m_listTask.SetItemText(i, 1, strTmp);

		CMyTask *pTask = (CMyTask*)(m_listTask.GetItemData(i));
		pTask->m_nIndex = i;
	}
}


void CPostFileDlg::OnBnClickedButtonDeal()
{
	CDlgDeal dlg;
	dlg.DoModal();
}
