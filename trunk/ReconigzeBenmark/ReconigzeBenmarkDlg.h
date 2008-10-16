// ReconigzeBenmarkDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "BlockFinder.h"


// CReconigzeBenmarkDlg 对话框
class CReconigzeBenmarkDlg : public CDialog
{
// 构造
public:
	CReconigzeBenmarkDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_RECONIGZEBENMARK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


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
	CSliderCtrl m_slider;
	BlockFinder m_finder;
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	afx_msg void OnBnClickedCancel();
};
