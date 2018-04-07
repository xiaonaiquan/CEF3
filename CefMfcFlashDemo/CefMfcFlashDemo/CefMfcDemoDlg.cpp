
// CefMfcDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CefMfcDemo.h"
#include "CefMfcDemoDlg.h"
#include "afxdialogex.h"

#include "include/cef_app.h"

#include "include/cef_sandbox_win.h"

#pragma comment(lib, ".\\cef_sandbox.lib")
#pragma comment(lib, "Version.lib")
#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "Ws2_32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CCefMfcDemoDlg 对话框

#define CEF_USE_SANDBOX 560

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

	void *sandbox_info = NULL;
#if defined(CEF_USE_SANDBOX)
	CefScopedSandboxInfo scoped_sandbox;
	sandbox_info = scoped_sandbox.sandbox_info();
#endif

	CefRefPtr<CSimpleClient> client(new CSimpleClient());
	m_simpleClient = client;

	CefMainArgs mainArgs;
	
	CefSettings settings;
	CefSettingsTraits::init(&settings);
	settings.multi_threaded_message_loop = true;
	settings.single_process = true;
	
	//CefRefPtr<CefApp> cefApp;
	
#if !defined(CEF_USE_SANDBOX)  
	settings.no_sandbox = true;
#endif  

	int exit_code = CefExecuteProcess(mainArgs, m_simpleClient.get(), sandbox_info);
	if (exit_code >= 0) {
		// The sub-process has completed so return here.  
		return exit_code;
	}

	CefInitialize(mainArgs, settings, m_simpleClient, sandbox_info);

	RECT rect;
	GetClientRect(&rect);
	RECT rectnew = rect;
	rectnew.top = rect.top + 50;
	rectnew.bottom = rect.bottom;
	rectnew.left = rect.left;
	rectnew.right = rect.right;

	CefWindowInfo winInfo;
	winInfo.SetAsChild(GetSafeHwnd(), rectnew);

	CefBrowserSettings browserSettings;
	CefBrowserHost::CreateBrowser(winInfo, client, _T("chrome://version"), browserSettings, NULL);

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
