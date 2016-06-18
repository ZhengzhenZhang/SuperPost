
// PostFileDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"




#define TIMER1	50		// ���½�����ʱ����


#include "MyToolTipCtrl.h"
#include "MyTask.h"

// CPostFileDlg �Ի���
class CPostFileDlg : public CDialogEx
{
// ����
public:
	CPostFileDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_POSTFILE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public:
	void ReadRecently();
	void WriteRecently();
	void UpdateListNum();		// �����б���ͼ���

	void AddTask(int nIndex);

public:
	CString m_strDragPath;
	int m_nPrgsIndex;
	CImageList m_ilList;		// 0 ��ʼ 1 ��ͣ 2 ֹͣ
	CProgressCtrl m_prgsTask;
	CListCtrl m_listTask;
	
	HACCEL m_hAccelTable;

	CMyToolTipCtrl m_ctlTT;

	CString m_strDirectory;		// ��ǰ����·��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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

