
// PostFileDlg.cpp : 实现文件
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


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CPostFileDlg 对话框



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


// CPostFileDlg 消息处理程序



BOOL CPostFileDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	// 设置列表视图布局
	m_ilList.Create(IDB_BITMAP_LIST, 16, 1, RGB(255, 0, 255));
	m_listTask.SetExtendedStyle(m_listTask.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	m_listTask.SetImageList(&m_ilList, LVSIL_SMALL);
	
	int nIndex = 0;
	m_listTask.InsertColumn(nIndex++, _T(""), LVCFMT_LEFT, 25);
	m_listTask.InsertColumn(nIndex++, _T(""), LVCFMT_LEFT, 26);
	m_listTask.InsertColumn(nIndex++, _T("文件名"), LVCFMT_LEFT, 150);
	m_listTask.InsertColumn(nIndex++, _T("目标IP"), LVCFMT_LEFT, 120);
	m_listTask.InsertColumn(nIndex++, _T("端口号"), LVCFMT_LEFT, 65);
	m_listTask.InsertColumn(nIndex++, _T("发送间隔"), LVCFMT_RIGHT, 65);
	m_listTask.InsertColumn(nIndex++, _T("包大小"), LVCFMT_RIGHT, 65);
	m_listTask.InsertColumn(nIndex++, _T("进度"), LVCFMT_RIGHT, 80);
	m_listTask.InsertColumn(nIndex++, _T("路径"), LVCFMT_LEFT, 500);

	// 允许拖拽文件
	DragAcceptFiles(TRUE);

	// 启动定时器
	SetTimer(1, TIMER1, NULL);

	// 加载快捷键
	m_hAccelTable = ::LoadAccelerators(AfxGetInstanceHandle(), 
		MAKEINTRESOURCE(IDR_ACCELERATORS));

	// 创建工具提示
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

	// 设置按钮图标
	((CButton*)GetDlgItem(IDC_BUTTON_BEGIN))->SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_BEGIN));
	((CButton*)GetDlgItem(IDC_BUTTON_STOP))->SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_STOP));
	((CButton*)GetDlgItem(IDC_BUTTON_SUSPEND))->SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_SUSPEND));
	((CButton*)GetDlgItem(IDC_BUTTON_LOOP))->SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_LOOP));
	((CButton*)GetDlgItem(IDC_BUTTON_DELETE))->SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_DELETE));
	((CButton*)GetDlgItem(IDC_BUTTON_DEAL))->SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_DEAL));

	// 得到当前软件所在目录
	TCHAR tcPath[MAX_PATH];
	::GetCurrentDirectory(sizeof (tcPath), tcPath);
	m_strDirectory = tcPath;

	// 读取最近播放列表
	ReadRecently();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPostFileDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
	// TODO: 在此添加控件通知处理程序代码

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
	if (IDCANCEL == MessageBox(_T("确定要退出？"), _T("SuperPost"), 
		MB_ICONQUESTION | MB_OKCANCEL))
		return;

	int nCounts = m_listTask.GetItemCount();

	for (int i = 0; i < nCounts; i++)
	{
		CMyTask *pTask = (CMyTask*)m_listTask.GetItemData(i);
		delete pTask;
	}

	// 写入最近播放列表
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
			if (IDCANCEL == MessageBox(_T("确定要停止选中任务？"), _T("SuperPost"), 
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
			if (IDCANCEL == MessageBox(_T("确定要删除选中任务？"), _T("SuperPost"), 
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
		// 得到最小进度任务
		CMyTask *pTask = (CMyTask*)m_listTask.GetItemData(i);
		bool bSend = pTask->m_strAddress != _T("###");

		if (bSend && !pTask->m_bStop && minPrgs > pTask->m_llPrgs)
			minPrgs = (int)pTask->m_llPrgs;

		// 刷新列表视图进度
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
			if (IDCANCEL == MessageBox(_T("确定要停止该任务并进行修改？"), _T("SuperPost"), 
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
