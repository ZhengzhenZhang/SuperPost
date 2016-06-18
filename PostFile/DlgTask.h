#pragma once
#include "afxwin.h"


// CDlgTask 对话框

class CDlgTask : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTask)

public:
	CDlgTask(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTask();

// 对话框数据
	enum { IDD = IDD_DLG_TASK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	void ReadRecently();
	void WriteRecently();

public:
	CString m_strName;
	CString m_strPath;
	CString m_strDIPP[4];		// 0 IP 1 端口 2 延时 3 包大小
	CComboBox m_cbDIPP[4];
	bool m_bModify;				// 是否处于修改状态
	CString m_strDirectory;		// 当前程序路径

	BOOL m_bReceive;

public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBnClickedButtonSelect();
	afx_msg void OnBnClickedCheckReceive();
};
