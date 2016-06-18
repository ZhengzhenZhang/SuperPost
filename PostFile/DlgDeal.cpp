// DlgDeal.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PostFile.h"
#include "DlgDeal.h"
#include "afxdialogex.h"


// CDlgDeal �Ի���

IMPLEMENT_DYNAMIC(CDlgDeal, CDialogEx)

CDlgDeal::CDlgDeal(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgDeal::IDD, pParent)
	, m_nRadio(0)
{
	m_bPrgcing = false;
}

CDlgDeal::~CDlgDeal()
{
}

void CDlgDeal::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_nRadio);
	DDX_Control(pDX, IDC_PROGRESS1, m_prgsCtrl1);
}


BEGIN_MESSAGE_MAP(CDlgDeal, CDialogEx)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


BOOL CDlgDeal::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ����Ӷ���ĳ�ʼ��

	DragAcceptFiles(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


// CDlgDeal ��Ϣ��������


void CDlgDeal::OnDropFiles(HDROP hDropInfo)
{
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ
	// ��ֹ�ظ��Ͻ�����
	if (m_bPrgcing)
		return;
	m_bPrgcing = true;

	// �õ��ļ�·��
	TCHAR fPath[MAX_PATH];
	DragQueryFile(hDropInfo, 0, fPath, sizeof (fPath));

	UpdateData(TRUE);

	switch (m_nRadio)
	{
	case 0:
		FunRadio1(fPath);
		break;

	case 1:
		FunRadio2(fPath);
		break;
	default:
		break;
	}

	m_prgsCtrl1.SetPos(0);

	m_bPrgcing = false;

	CDialogEx::OnDropFiles(hDropInfo);
}


bool CDlgDeal::PeekAndPump()
{
	MSG msg;
	while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
		if (!AfxGetApp()->PumpMessage()) {
			::PostQuitMessage(0);
			return false;
		}
	}
	LONG lIdle = 0;
	while (AfxGetApp()->OnIdle(lIdle++));
	return true;
}

#define DTOHEX(a) ((a) < 10 ? ((a) + _T('0')) : ((a)-10 + 'A'))
void CDlgDeal::FunRadio1(LPCTSTR fPath)
{
	CString strPath = fPath;
	strPath += _T("_HEX");
	

	CFile fileIn;
	fileIn.Open(fPath, CFile::modeRead);

	CStdioFile fileOut;
	fileOut.Open(strPath, CFile::modeReadWrite | CFile::modeCreate);

	// ������ȱ���
	ULONGLONG nTotal = fileIn.GetLength();
	ULONGLONG nNowPro = 0;
	int nPrograss = 0;

	BYTE rBuf[1005];		// ��������
	BYTE wBuf[3005];		// д������

	int perSize = 1000;
	int len = 0;
	while (len = fileIn.Read(&rBuf, perSize))
	{
		// ����������
		nNowPro +=  perSize;
		int tmpPgrs= (int)((nNowPro*100.0) / nTotal + 0.5); 
		if (tmpPgrs != nPrograss)
		{
			nPrograss = tmpPgrs;
			m_prgsCtrl1.SetPos(nPrograss);
		}

		// ת������
		for (int i = 0; i < len; i++)
		{
			int j = i * 3;

			wBuf[j] = DTOHEX((rBuf[i] & 0xf0) >> 4);
			wBuf[j + 1] = DTOHEX(rBuf[i] & 0x0f);
			wBuf[j + 2] = _T(' ');
		}

		// д������ļ�
		fileOut.Write(wBuf, len*3);

		if (!PeekAndPump())
			break;
	}

	fileIn.Close();
	fileOut.Close();
	m_prgsCtrl1.SetPos(100);

	CString str = _T("ת���ɹ���\n");
	str += strPath;
	MessageBox(str, NULL, MB_ICONINFORMATION);
}

#define HEXTOD(a) ((a) <= _T('9') ? ((a) - _T('0')) : ((a) - 'A' + 10))
#define LOTOUP(a) ((a) >= _T('a') ? (a)-_T('a')+_T('A') : (a))
void CDlgDeal::FunRadio2(LPCTSTR fPath)
{
	CString strPath = fPath;
	strPath += _T("_FILE");


	CFile fileIn;
	fileIn.Open(fPath, CFile::modeRead);

	CStdioFile fileOut;
	fileOut.Open(strPath, CFile::modeReadWrite | CFile::modeCreate);

	// ������ȱ���
	ULONGLONG nTotal = fileIn.GetLength();
	ULONGLONG nNowPro = 0;
	int nPrograss = 0;

	BYTE rBuf[3005];		// ��������
	BYTE wBuf[1005];				// д������

	int perSize = 3000;
	int len = 0;
	while (len = fileIn.Read(&rBuf, perSize))
	{
		// ����������
		nNowPro +=  perSize;
		int tmpPgrs= (int)((nNowPro*100.0) / nTotal + 0.5); 
		if (tmpPgrs != nPrograss)
		{
			nPrograss = tmpPgrs;
			m_prgsCtrl1.SetPos(nPrograss);
		}

		// ת������
		for (int i = 0; i < len; i+=3)
		{
			char hi = LOTOUP(rBuf[i]);
			char low = LOTOUP(rBuf[i + 1]);
			wBuf[i/3] = HEXTOD(hi) * 16 + HEXTOD(low);
		}

		// д������ļ�
		fileOut.Write(wBuf, len/3);

		if (!PeekAndPump())
			break;
	}

	fileIn.Close();
	fileOut.Close();
	m_prgsCtrl1.SetPos(100);

	CString str = _T("ת���ɹ���\n");
	str += strPath;
	MessageBox(str, NULL, MB_ICONINFORMATION);
}
