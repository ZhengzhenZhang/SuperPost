#pragma once
#include "afxcmn.h"


// CDlgDeal 对话框

class CDlgDeal : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgDeal)

public:
	CDlgDeal(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDeal();

// 对话框数据
	enum { IDD = IDD_DLG_DEAL };

public:
	bool PeekAndPump();				// 消息泵
	void FunRadio1(LPCTSTR fPath);
	void FunRadio2(LPCTSTR fPath);
public:
	int m_nRadio;
	CProgressCtrl m_prgsCtrl1;
	bool m_bPrgcing;			// 防止重复拖进数据
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	virtual BOOL OnInitDialog();
};
