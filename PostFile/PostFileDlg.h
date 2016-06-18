
// PostFileDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"




#define TIMER1	50		// 更新进度条时间间隔


#include "MyToolTipCtrl.h"
#include "MyTask.h"

// CPostFileDlg 对话框
class CPostFileDlg : public CDialogEx
{
// 构造
public:
	CPostFileDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_POSTFILE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	void ReadRecently();
	void WriteRecently();
	void UpdateListNum();		// 更新列表视图编号

	void AddTask(int nIndex);

public:
	CString m_strDragPath;
	int m_nPrgsIndex;
	CImageList m_ilList;		// 0 开始 1 暂停 2 停止
	CProgressCtrl m_prgsTask;
	CListCtrl m_listTask;
	
	HACCEL m_hAccelTable;

	CMyToolTipCtrl m_ctlTT;

	CString m_strDirectory;		// 当前程序路径

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnBnClickedButtonAddtask();
	afx_msg void OnNMDblclkListTask(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDropFiles(HDROP hDropInfo);

	virtual void OnCancel();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);

	afx_msg void OnMenuBegin();
	afx_msg void OnMenuSuspend();
	afx_msg void OnMenuStop();
	afx_msg void OnMenuDelete();
	afx_msg void OnMenuLoop();
	afx_msg void OnOtherSelall();
	afx_msg void OnOtherSpace();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnMenuModify();
	afx_msg void OnDestroy();
};

