
// CefMfcDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CefMfcDemo.h"
#include "CefMfcDemoDlg.h"
#include "afxdialogex.h"

#include "include/cef_app.h"
#include "ClientAppRender.h"
#include "include/cef_runnable.h"
#include "include/cef_cookie.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCefMfcDemoDlg 对话框

CCefMfcDemoDlg::CCefMfcDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCefMfcDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCefMfcDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCefMfcDemoDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_GO, &CCefMfcDemoDlg::OnBnClickedBtnGo)
	ON_BN_CLICKED(IDC_BTN_GOFORD, &CCefMfcDemoDlg::OnBnClickedBtnGoford)
	ON_BN_CLICKED(IDC_BTN_GOBACK, &CCefMfcDemoDlg::OnBnClickedBtnGoback)
END_MESSAGE_MAP()


// CCefMfcDemoDlg 消息处理程序


BOOL CCefMfcDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	CefRefPtr<CSimpleClient> client(new CSimpleClient());
	m_simpleClient = client;

	CefRefPtr<ClientAppRender> app(new ClientAppRender());


	CefSettings settings;
	CefSettingsTraits::init(&settings);
	settings.multi_threaded_message_loop = true;
	settings.remote_debugging_port = 8088;//如果不定义，则不能运行调试工具
	settings.single_process = false;

	CefMainArgs mainArgs;

	CefRefPtr<CefApp> cefApp;
	cefApp = app;

	CefInitialize(mainArgs, settings, cefApp, NULL);

	RECT rect;
	GetClientRect(&rect);
	RECT rectnew = rect;
	rectnew.top = rect.top + 50;
	rectnew.bottom = rect.bottom;
	rectnew.left = rect.left;
	rectnew.right = rect.right;

	CefWindowInfo winInfo;
	winInfo.SetAsChild(GetSafeHwnd(), rectnew);

	CefRefPtr<CefSetCookieCallback> callback = NULL;
	std::wstring username_key = L"Username"; //cookie的key
	std::wstring username_value = L"xiaonaiquan";//cookie的value
	std::wstring domain = L"csdn.net";

	CefRefPtr<CefCookieManager> manager = CefCookieManager::GetGlobalManager(nullptr);
	CefCookie cookie;
	CefString(&cookie.name).FromWString(username_key.c_str());
	CefString(&cookie.value).FromWString(username_value.c_str());
	CefString(&cookie.domain).FromWString(domain.c_str());
	CefString(&cookie.path).FromASCII("/");
	cookie.has_expires = false;

	domain = L"https://" + domain;
	CefPostTask(TID_IO, NewCefRunnableMethod(manager.get(), &CefCookieManager::SetCookie, CefString(domain.c_str()), cookie, callback));

	CefBrowserSettings browserSettings;
	CefBrowserHost::CreateBrowser(winInfo, m_simpleClient, domain.c_str(), browserSettings, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCefMfcDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCefMfcDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCefMfcDemoDlg::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CefShutdown();

	CDialogEx::OnClose();
}



void CCefMfcDemoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	RECT rect;
	GetClientRect(&rect);

	if (m_simpleClient.get())
	{
		CefRefPtr<CefBrowser> browser = m_simpleClient->GetBrowser();
		if (browser)
		{
			CefWindowHandle hwnd = browser->GetHost()->GetWindowHandle();
			::MoveWindow(hwnd, 0, 50, rect.right - rect.left, rect.bottom - 50, true);
		}
	}
}

/*
	Go按钮
*/
void CCefMfcDemoDlg::OnBnClickedBtnGo()
{
	// TODO:  在此添加控件通知处理程序代码
	CString strUrl;
	GetDlgItem(IDC_EDIT_URL)->GetWindowText(strUrl);
	if (strUrl.Trim().IsEmpty())
	{
		AfxMessageBox(_T("请输入网址"));
		return;
	}
	const CefString cefStrUrl(strUrl);
	m_simpleClient->GetBrowser()->GetMainFrame()->LoadURL(cefStrUrl);
}

/*
	前进按钮
*/
void CCefMfcDemoDlg::OnBnClickedBtnGoford()
{
	// TODO:  在此添加控件通知处理程序代码
	m_simpleClient->GetBrowser()->GoForward();
}

/*
	后退按钮
*/
void CCefMfcDemoDlg::OnBnClickedBtnGoback()
{
	// TODO:  在此添加控件通知处理程序代码
	m_simpleClient->GetBrowser()->GoBack();
}

/*
	响应回车键
*/
BOOL CCefMfcDemoDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:    // 屏蔽回车
			OnBnClickedBtnGo();
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
