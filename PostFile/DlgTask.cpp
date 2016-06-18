// DlgTask.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PostFile.h"
#include "DlgTask.h"
#include "afxdialogex.h"


// CDlgTask �Ի���

IMPLEMENT_DYNAMIC(CDlgTask, CDialogEx)

CDlgTask::CDlgTask(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgTask::IDD, pParent)
	, m_strPath(_T(""))
	, m_bReceive(FALSE)
{
	m_strName = _T("");

	for (int i = 0; i < 4; i++)
		m_strDIPP[i] = _T("");

	m_bModify = 0;

	m_strDirectory = _T("");
}

CDlgTask::~CDlgTask()
{
}

void CDlgTask::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PATH, m_strPath);
	DDX_CBString(pDX, IDC_COMBO_IP, m_strDIPP[0]);
	DDX_CBString(pDX, IDC_COMBO_PORT, m_strDIPP[1]);
	DDX_CBString(pDX, IDC_COMBO_DELAY, m_strDIPP[2]);
	DDX_CBString(pDX, IDC_COMBO_PERSIZE, m_strDIPP[3]);

	DDX_Control(pDX, IDC_COMBO_IP, m_cbDIPP[0]);
	DDX_Control(pDX, IDC_COMBO_PORT, m_cbDIPP[1]);
	DDX_Control(pDX, IDC_COMBO_DELAY, m_cbDIPP[2]);
	DDX_Control(pDX, IDC_COMBO_PERSIZE, m_cbDIPP[3]);
	DDX_Check(pDX, IDC_CHECK_RECEIVE, m_bReceive);
}


BEGIN_MESSAGE_MAP(CDlgTask, CDialogEx)

	ON_BN_CLICKED(IDC_BUTTON_SELECT, &CDlgTask::OnBnClickedButtonSelect)
	ON_BN_CLICKED(IDC_CHECK_RECEIVE, &CDlgTask::OnBnClickedCheckReceive)
END_MESSAGE_MAP()


// CDlgTask ��Ϣ�������

BOOL CDlgTask::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	if (m_strPath != _T(""))
	{
		LPCTSTR pTch = m_strPath.GetBuffer(m_strPath.GetLength());

		int x = m_strPath.GetLength() - 1;
		int n = 0;

		while (1)
		{
			if (_T('\\') == pTch[x--])
				break;
			n++;
		}

		m_strName = m_strPath.Right(n);
	}

	if (m_bModify)
	{
		SetWindowText(_T("�޸�����"));
		GetDlgItem(IDOK)->SetWindowText(_T("�޸�"));
		GetDlgItem(IDC_EDIT_PATH)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SELECT)->EnableWindow(FALSE);
	}
	
	// ����޸�״̬�Ͳ���ȡ�������

	ReadRecently();

	// �õ�����޸�����
	if (!m_bModify)
	{
		for (int i = 0; i < 4; i++)
			if (m_cbDIPP[i].GetCount() > 0)
				m_cbDIPP[i].GetLBText(0, m_strDIPP[i]);
	}
	
	UpdateData(FALSE);

	OnBnClickedCheckReceive();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgTask::OnBnClickedButtonSelect()
{
	CFileDialog fileDlg(!m_bReceive);

	if (IDCANCEL == fileDlg.DoModal())
		return;

	m_strPath = fileDlg.GetPathName();
	m_strName = fileDlg.GetFileName();

	GetDlgItem(IDC_EDIT_PATH)->SetWindowText(m_strPath);

	GetDlgItem(IDOK)->SetFocus();
}


void CDlgTask::ReadRecently()
{
	if (m_bReceive)
		return;
	CStdioFile stdFile;
	CString strPath = m_strDirectory + _T("\\SuperPost\\config_set.rct");

	if (stdFile.Open(strPath, CFile::modeRead))
	{
		CString strRead;
		while (stdFile.ReadString(strRead))
		{
			int nX;
			TCHAR tX[100];
			_stscanf_s(strRead, _T("%d %s"), &nX, tX, sizeof (tX));

			if (nX >= 0 && nX <= 3)
				m_cbDIPP[nX].AddString(tX);
		}

		stdFile.Close();
	}
}


void CDlgTask::WriteRecently()
{ 
	if (m_bReceive)
		return;
	// �����ʹ�õĲ��뵽��һ��
	for (int i = 0; i < 4; i++)
	{
		int nIndex = m_cbDIPP[i].FindStringExact(-1, m_strDIPP[i]);

		if (nIndex >= 0)
			m_cbDIPP[i].DeleteString(nIndex);

		m_cbDIPP[i].InsertString(0, m_strDIPP[i]);

		while (nIndex = m_cbDIPP[i].GetCount())
		{
			if (nIndex <= 5)
				break;
			m_cbDIPP[i].DeleteString(nIndex - 1);
		}
	}
	
	UpdateData(FALSE);

	CStdioFile stdFile;
	CString strPath = m_strDirectory;
	strPath += _T("\\SuperPost");
	::CreateDirectory(strPath, NULL);
	strPath += _T("\\config_set.rct");

	if (stdFile.Open(strPath, CFile::modeWrite | CFile::modeCreate))
	{
		CString strWrite;
		for (int i = 0; i < 4; i++)
		{
			int n = m_cbDIPP[i].GetCount();
			for (int j = 0; j < n; j++)
			{
				CString strTmp;
				m_cbDIPP[i].GetLBText(j, strTmp);

				strWrite.Format(_T("%d %s\n"), i, strTmp);
			
				stdFile.WriteString(strWrite);
			}
		}
		stdFile.Close();
	}
}


void CDlgTask::OnOK()
{
	UpdateData(TRUE);

	if (_T("") == m_strDIPP[0] && !m_bReceive)
	{
		AfxMessageBox(_T("Ŀ��IP����Ϊ�գ�"));
		return;
	}
	if (_ttoi(m_strDIPP[1]) < 0 || _ttoi(m_strDIPP[1]) > 65535)
	{
		AfxMessageBox(_T("�˿ںŴ���"));
		return;
	}
	if ((_ttoi(m_strDIPP[2]) < 0 || _ttoi(m_strDIPP[2]) > 3600000)
		&& !m_bReceive)
	{
		AfxMessageBox(_T("��������ʵ�ʱ������"));
		return;
	}
	if ((_ttoi(m_strDIPP[3]) <= 0 || _ttoi(m_strDIPP[3]) > 1000000)
		&& !m_bReceive)
	{
		AfxMessageBox(_T("�������С��ΧΪ 1 - 1000000��"));
		return;
	}

	if (_T("") == m_strPath)
	{
		AfxMessageBox(_T("�ļ�·������Ϊ�գ�"));
		return;
	}

	if (m_bReceive)
	{
		m_strDIPP[0] = _T("###");
		m_strDIPP[2] = _T("###");
		m_strDIPP[3] = _T("###");
		UpdateData(FALSE);
	}
	
	WriteRecently();

	CDialogEx::OnOK();
}


void CDlgTask::OnBnClickedCheckReceive()
{
	UpdateData(TRUE);

	if (m_bReceive)
	{
		GetDlgItem(IDC_STATIC_PORT)->SetWindowText(_T("���ض˿ڣ�"));
		GetDlgItem(IDC_COMBO_IP)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_DELAY)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_PERSIZE)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_STATIC_PORT)->SetWindowText(_T("Ŀ��˿ڣ�"));
		GetDlgItem(IDC_COMBO_IP)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_DELAY)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_PERSIZE)->EnableWindow(TRUE);
	}
}
