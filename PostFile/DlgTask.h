#pragma once
#include "afxwin.h"


// CDlgTask �Ի���

class CDlgTask : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTask)

public:
	CDlgTask(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgTask();

// �Ի�������
	enum { IDD = IDD_DLG_TASK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	void ReadRecently();
	void WriteRecently();

public:
	CString m_strName;
	CString m_strPath;
	CString m_strDIPP[4];		// 0 IP 1 �˿� 2 ��ʱ 3 ����С
	CComboBox m_cbDIPP[4];
	bool m_bModify;				// �Ƿ����޸�״̬
	CString m_strDirectory;		// ��ǰ����·��

	BOOL m_bReceive;

public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBnClickedButtonSelect();
	afx_msg void OnBnClickedCheckReceive();
};
