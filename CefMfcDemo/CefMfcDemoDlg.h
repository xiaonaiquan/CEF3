
// CefMfcDemoDlg.h : 头文件
//
// http://blog.csdn.net/zhuhongshu/article/details/70159672
// 在很多版本的Cef里，如果开启了多线程消息循环，会导致程序在结束时触发中断，这属于Cef的bug，不过在release版本的Cef中没有问题。
// 在2623、2526版本，Debug模式中，如果开启了multi_threaded_message_loop，当程序退出时，必定会触发中断。这个属于Cef的bug，在官方demo中也有这个问题，但是在Release模式中是没有问题的。
// 如果在使用Cef模块中遇到一些崩溃或者其他异常现象，请先使用release模式+开启多进程模式再运行一次，很多问题都是debug模式或者单进程模式导致的

#pragma once

#include "SimpleClient.h"

// CCefMfcDemoDlg 对话框
class CCefMfcDemoDlg : public CDialogEx
{
// 构造
public:
	CCefMfcDemoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CEFMFCDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	CefRefPtr<CSimpleClient>  m_simpleClient;

	BOOL PreTranslateMessage(MSG* pMsg);

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnGo();
	afx_msg void OnBnClickedBtnGoford();
	afx_msg void OnBnClickedBtnGoback();
};
