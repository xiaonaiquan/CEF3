#include "SimpleClient.h"
#include  <tchar.h>

enum { ID_CMD_REFRESH = 0 ,
	ID_SHOW_DEV_TOOLS
};

CSimpleClient::CSimpleClient()
{
	hWnd_ = NULL;
}


CSimpleClient::~CSimpleClient()
{
}

void CSimpleClient::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
	if (!m_cefBrowser.get())
	{
		m_cefBrowser = browser;
		m_BrowserHwnd = browser->GetHost()->GetWindowHandle();
	}
}

//void CSimpleClient::OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line)
//{
//
//	command_line->AppendSwitch("no-proxy-server");
//	command_line->AppendSwitch("--enable-npapi");
//	command_line->AppendSwitch("--disable-web-security");
//	command_line->AppendSwitch("allow-outdated-plugins");
//
//	//manifest.json中的version
//	command_line->AppendSwitchWithValue("ppapi-flash-version","20.0.0.228");
//
//	//加载flash插件
//	command_line->AppendSwitchWithValue("ppapi-flash-path", "PepperFlash\\pepflashplayer.dll");
//
//}

bool CSimpleClient::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
	CefProcessId source_process,
	CefRefPtr<CefProcessMessage> message)
{
	const std::string& messageName = message->GetName();
	if (messageName == "login_msg")
	{
		// extract message
		CefRefPtr<CefListValue> args = message->GetArgumentList();
		CefString strUser = args->GetString(0);
		CefString strPassword = args->GetString(1);
		TCHAR szLog[256] = { 0 };
		_stprintf_s(szLog, 256, L"BrowserProcess, user - %s, password - %s\r\n", strUser.c_str(), strPassword.c_str());
		OutputDebugString(szLog);

		//send reply to render process
		CefRefPtr<CefProcessMessage> outMsg = CefProcessMessage::Create("login_reply");

		// Retrieve the argument list object.
		CefRefPtr<CefListValue> replyArgs = outMsg->GetArgumentList();

		// Populate the argument values.
		replyArgs->SetSize(1);
		replyArgs->SetInt(0, 0);

		// Send the process message to the renderer process.
		browser->SendProcessMessage(PID_RENDERER, outMsg);

		return true;
	}
	return false;
}

bool CSimpleClient::OnBeforePopup(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	const CefString& target_url,
	const CefString& target_frame_name,
	WindowOpenDisposition target_disposition,
	bool user_gesture,
	const CefPopupFeatures& popupFeatures,
	CefWindowInfo& windowInfo,
	CefRefPtr<CefClient>& client,
	CefBrowserSettings& settings,
	bool* no_javascript_access)
{
	switch (target_disposition)
	{
	case WOD_NEW_FOREGROUND_TAB:
	case WOD_NEW_BACKGROUND_TAB:
	case WOD_NEW_POPUP:
	case WOD_NEW_WINDOW:
		browser->GetMainFrame()->LoadURL(target_url);
		return true; //cancel create
	}

	return false;
}

void CSimpleClient::OnBeforeDownload(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefDownloadItem> download_item,
	const CefString& suggested_name,
	CefRefPtr<CefBeforeDownloadCallback> callback)
{
	callback->Continue(download_item->GetURL(), true);
}

void CSimpleClient::OnDownloadUpdated(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefDownloadItem> download_item,
	CefRefPtr<CefDownloadItemCallback> callback)
{
	if (download_item->IsComplete())
	{
		//MessageBox.Show("下载成功");
		OutputDebugString(L"下载成功");
		if (browser->IsPopup() && !browser->HasDocument())
		{
			//browser->GetHost()->ParentWindowWillClose();
			browser->GetHost()->CloseBrowser(true);
		}
	}
	else
	{
		//如果取消应该跳转到一个网页
		browser->GoBack();
	}
}

bool CSimpleClient::OnPreKeyEvent(CefRefPtr<CefBrowser> browser,
	const CefKeyEvent& event,
	CefEventHandle os_event,
	bool* is_keyboard_shortcut) 
{
	if (event.type != KEYEVENT_RAWKEYDOWN)
		return false;

	if (event.windows_key_code == 116)//F5刷新
		browser->Reload();
	else if (event.windows_key_code == VK_F12)
	{
		ShowDevelopTools(browser, CefPoint(0, 0));
	}
	return false;
}

bool CSimpleClient::OnKeyEvent(CefRefPtr<CefBrowser> browser,
	const CefKeyEvent& event,
	CefEventHandle os_event) {
	return false;
}

//右键菜单
void CSimpleClient::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefContextMenuParams> params,
	CefRefPtr<CefMenuModel> model) 
{
	if (model->GetCount() > 0) {// 刷新菜单
		model->AddSeparator();
		model->AddItem(ID_CMD_REFRESH, __TEXT("刷新"));
		model->AddItem(ID_SHOW_DEV_TOOLS, __TEXT("开发者选项"));
	}
}

bool CSimpleClient::OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefContextMenuParams> params,
	int command_id, EventFlags event_flags) 
{
	switch (command_id)
	{
	case ID_CMD_REFRESH:
		browser->Reload();
		break;
	case ID_SHOW_DEV_TOOLS:
	{
		ShowDevelopTools(browser, CefPoint(0,0));
		break;
	}
	default:
		break;
	}
	return false;
}

//开发选项
void CSimpleClient::ShowDevelopTools(CefRefPtr<CefBrowser> browser, const CefPoint& inspect_element_at) 
{
	CefWindowInfo windowInfo;
	CefBrowserSettings settings;
	windowInfo.SetAsPopup(browser->GetHost()->GetWindowHandle(), "DevTools");
	browser->GetHost()->ShowDevTools(windowInfo, this, settings, CefPoint());
}

void CSimpleClient::CloseDevelopTools(CefRefPtr<CefBrowser> browser)
{
	browser->GetHost()->CloseDevTools();
}

bool CSimpleClient::DoClose(CefRefPtr<CefBrowser> browser)
{
	////同一个browser可能有多个浏览器窗口(在新的子窗口打开链接,而不是在当前窗口跳转时会出现.),此时返回值可能会被用来做一些特别处理.想了解请参考官方demo.
	//if (CefCurrentlyOn(TID_UI))
	//{
	//	return true;
	//}
	return false;	//返回true取消关闭
}
void CSimpleClient::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
	if (m_BrowserHwnd == browser->GetHost()->GetWindowHandle())
	{// 浏览器窗口被销毁的话,browser指针置为NULL.
		m_cefBrowser = NULL;
	}

}







