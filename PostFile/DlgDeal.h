#pragma once
#include "afxcmn.h"


// CDlgDeal �Ի���

class CDlgDeal : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgDeal)

public:
	CDlgDeal(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgDeal();

// �Ի�������
	enum { IDD = IDD_DLG_DEAL };

public:
	bool PeekAndPump();				// ��Ϣ��
	void FunRadio1(LPCTSTR fPath);
	void FunRadio2(LPCTSTR fPath);
public:
	int m_nRadio;
	CProgressCtrl m_prgsCtrl1;
	bool m_bPrgcing;			// ��ֹ�ظ��Ͻ�����
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	virtual BOOL OnInitDialog();
};
